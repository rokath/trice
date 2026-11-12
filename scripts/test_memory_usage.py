"""Check byte accounting and baseline differences without a compiler or network."""

from pathlib import Path
import io
import shlex
import shutil
import subprocess
import tarfile
import tempfile
import unittest
from unittest.mock import patch

from memory_usage import (TARGETS, arguments, check_build_plan, command, comparison_table,
                          clang_make_options, linker_values, membrowse_target, overlay_sources,
                          section_sizes, series_name)


# The data load image is in Flash, while its variables live in RAM.
LINKER = """
_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;
MEMORY {
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 8K
  FLASH (rx) : ORIGIN = 0x8000000, LENGTH = 64K
}
"""
SECTIONS = """
Idx Name              Size      VMA       LMA       File off  Algn
  0 .isr_vector       000000c0  08000000  08000000  00001000  2**0
                      CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .text             00000100  080000c0  080000c0  000010c0  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
  2 .data             0000000c  20000000  080001c0  00002000  2**2
                      CONTENTS, ALLOC, LOAD, DATA
  3 .bss              00000010  2000000c  080001cc  0000200c  2**2
                      ALLOC
  4 ._user_heap_stack 00000604  2000001c  080001cc  0000200c  2**0
                      ALLOC
  5 .debug_info       00000300  00000000  00000000  0000200c  2**0
                      CONTENTS, READONLY, DEBUGGING, OCTETS
"""


class MemoryAccountingTests(unittest.TestCase):
    """Keep storage, linker reservations and debug information distinct."""

    def test_initialized_data_uses_both_regions_without_counting_heap_as_static(self):
        result = section_sizes(SECTIONS, linker_values(LINKER))
        self.assertEqual(result, dict(flash=460, static_ram=28, reserved_ram=1540,
                                      heap_min=512, stack_min=1024))

    def test_zero_size_sections_at_region_end_are_harmless(self):
        empty = "  6 .empty 00000000 20002000 20002000 0000200c 2**0\n ALLOC\n"
        self.assertEqual(section_sizes(SECTIONS + empty, linker_values(LINKER))["static_ram"], 28)

    def test_new_static_section_is_counted_without_a_section_name_allowlist(self):
        noinit = "  6 .noinit 00000004 20001000 20001000 0000200c 2**0\n ALLOC\n"
        self.assertEqual(section_sizes(SECTIONS + noinit, linker_values(LINKER))["static_ram"], 32)

    def test_section_can_end_exactly_at_the_ram_limit(self):
        noinit = "  6 .noinit 00000004 20001ffc 20001ffc 0000200c 2**0\n ALLOC\n"
        self.assertEqual(section_sizes(SECTIONS + noinit, linker_values(LINKER))["static_ram"], 32)

    def test_sections_cannot_extend_beyond_a_region(self):
        for replacement in ("20001fff", "30000000"):
            with self.subTest(address=replacement), self.assertRaisesRegex(ValueError, "does not fit"):
                section_sizes(SECTIONS.replace("2000000c", replacement), linker_values(LINKER))

    def test_flash_load_image_must_fit_even_when_ram_address_fits(self):
        with self.assertRaisesRegex(ValueError, "Load image for .data"):
            section_sizes(SECTIONS.replace("080001c0", "0800fffc"), linker_values(LINKER))

    def test_missing_reservation_or_malformed_output_is_rejected(self):
        for text in ("", "not an objdump table", SECTIONS.replace("00000604", "00000004")):
            with self.subTest(text=text), self.assertRaises(ValueError):
                section_sizes(text, linker_values(LINKER))

    def test_nonliteral_or_missing_linker_values_require_review(self):
        for text in (LINKER.replace("8K", "RAM_SIZE"), LINKER.replace("_Min_Heap_Size", "other")):
            with self.subTest(text=text), self.assertRaises(ValueError):
                linker_values(text)

    def test_deltas_use_bare_even_if_rows_are_reordered(self):
        rows = [dict(target="F030_inst_rtt", flash=110, static_ram=8),
                dict(target="F030_bare", flash=100, static_ram=10),
                dict(target="F030_inst_uart", flash=130, static_ram=30)]
        lines = comparison_table(rows).splitlines()
        self.assertEqual([value.strip() for value in lines[2].split("|")],
                         ["F030_inst_rtt", "+10", "-2", "110", "8"])
        self.assertEqual([value.strip() for value in lines[3].split("|")],
                         ["F030_bare", "+0", "+0", "100", "10"])
        self.assertEqual([value.strip() for value in lines[4].split("|")],
                         ["F030_inst_uart", "+30", "+20", "130", "30"])


class ComparisonSeriesTests(unittest.TestCase):
    """Keep compiler/framing variants comparable without firmware execution."""

    def test_defaults_enable_lto_and_keep_tcobs_and_gcc(self):
        options = arguments([])
        self.assertEqual(series_name(options), "gcc-tcobs-lto")
        self.assertFalse(options.worktree)

    def test_all_three_linker_scripts_have_identical_memory_layouts(self):
        root = Path(__file__).resolve().parent.parent
        scripts = [(root / "examples" / target / "STM32F030R8Tx_FLASH.ld").read_bytes()
                   for target in TARGETS]
        self.assertEqual(scripts[0], scripts[1])
        self.assertEqual(scripts[0], scripts[2])
        self.assertEqual(linker_values(scripts[0].decode("utf-8")),
                         dict(FLASH=(0x08000000, 65536), RAM=(0x20000000, 8192), Heap=512, Stack=1024))

    def test_dotted_windows_tool_name_uses_the_resolved_executable(self):
        # Simulate Windows PATHEXT lookup without depending on the actual host.
        selected = str(Path("tool chain") / "ld.lld.exe")
        with patch("memory_usage.shutil.which", return_value=selected) as lookup:
            with patch("memory_usage.subprocess.run") as run:
                command(["ld.lld", "--version"], capture=True, env={"PATH": "selected tool path"})
        lookup.assert_called_once_with("ld.lld", path="selected tool path")
        self.assertEqual(run.call_args.args[0], [selected, "--version"])

    def test_missing_tool_reports_the_required_executable(self):
        with patch("memory_usage.shutil.which", return_value=None):
            with self.assertRaisesRegex(ValueError, "missing from PATH: ld.lld"):
                command(["ld.lld", "--version"])

    def test_clang_retains_gnu_startup_and_runtime_paths_with_spaces(self):
        with tempfile.TemporaryDirectory(prefix="memory runtime ") as temporary:
            root = Path(temporary)
            runtime = root / "arm-none-eabi/lib/thumb/v6-m/nofp"
            runtime.mkdir(parents=True)
            include = root / "arm-none-eabi/include"
            include.mkdir()
            (include / "stdlib.h").write_text("/* fixture Newlib header */\n")
            resources = {}
            for name in ("crti.o", "crtbegin.o", "crt0.o", "crtend.o", "crtn.o",
                         "libc_nano.a", "libm.a", "libnosys.a", "libgcc.a"):
                path = runtime / name
                path.write_bytes(b"fixture runtime")
                resources[name] = path.as_posix()

            def resolve(arguments, **unused):
                """Model GCC's native resource lookup, without launching tools."""
                return resources[arguments[-1].split("=", 1)[1]]

            with patch("memory_usage.command", side_effect=resolve):
                options = clang_make_options()
            compiler = shlex.split(next(value[3:] for value in options if value.startswith("CC=")))
            self.assertEqual(compiler[-2:], ["-isystem", include.as_posix()])
            link = shlex.split(next(value[7:] for value in options if value.startswith("LFLAGS=")))
            self.assertTrue(set(resources.values()).issubset(link))
            self.assertIn("-fuse-ld=lld", link)

    @unittest.skipUnless(shutil.which("tar"), "tar is required for the committed source snapshot")
    def test_snapshot_extraction_accepts_native_paths_with_spaces(self):
        # Exercise the real tar on this host. Relative arguments work with
        # MSYS, GNU and BSD tar without Windows drive/remote-host ambiguity.
        with tempfile.TemporaryDirectory(prefix="memory usage ") as temporary:
            root = Path(temporary)
            archive, destination = root / "source.tar", root / "source snapshot"
            destination.mkdir()
            payload = b"committed source\n"
            with tarfile.open(archive, "w") as stream:
                entry = tarfile.TarInfo("cube.txt")
                entry.size = len(payload)
                stream.addfile(entry, io.BytesIO(payload))
            command(["tar", "-xf", archive.name, "-C", destination.name], cwd=root)
            self.assertEqual((destination / "cube.txt").read_bytes(), payload)

    def test_artifacts_are_separate_and_only_uart_gets_a_framing_target(self):
        names = set()
        for compiler in ("gcc", "clang"):
            for lto in ("on", "off"):
                tcobs = arguments(["--compiler", compiler, "--lto", lto])
                cobs = arguments(["--compiler", compiler, "--lto", lto, "--framing", "cobs"])
                self.assertNotEqual(series_name(tcobs), series_name(cobs))
                for target in TARGETS[:2]:
                    self.assertEqual(membrowse_target(target, tcobs), membrowse_target(target, cobs))
                self.assertNotEqual(membrowse_target(TARGETS[2], tcobs), membrowse_target(TARGETS[2], cobs))
                names.update(membrowse_target(target, tcobs) for target in TARGETS)
                names.add(membrowse_target(TARGETS[2], cobs))
        self.assertEqual(len(names), 16)

    @unittest.skipUnless(shutil.which("make"), "GNU Make is required for the example build plans")
    def test_all_three_projects_apply_lto_and_framing_synchronously(self):
        root = Path(__file__).resolve().parent.parent
        for lto in ("on", "off"):
            for framing in ("tcobs", "cobs"):
                options = arguments(["--lto", lto, "--framing", framing])
                for target in TARGETS:
                    with self.subTest(target=target, lto=lto, framing=framing):
                        plan = subprocess.check_output(
                            ["make", "-n", "-B", "-C", f"examples/{target}",
                             f"LTO={int(lto == 'on')}", "OPT=-Og",
                             f"TRICE_FLAGS=-DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_{framing.upper()}"],
                            cwd=root, text=True, encoding="utf-8",
                        )
                        check_build_plan(plan, target, options)

    @unittest.skipUnless(shutil.which("make"), "GNU Make is required for the example build plans")
    def test_all_three_projects_enable_lto_without_an_override(self):
        root = Path(__file__).resolve().parent.parent
        for target in TARGETS:
            with self.subTest(target=target):
                plan = subprocess.check_output(
                    ["make", "-n", "-B", "-C", f"examples/{target}",
                     "TRICE_FLAGS=-DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_TCOBS"],
                    cwd=root, text=True, encoding="utf-8",
                )
                check_build_plan(plan, target, arguments([]))

    def test_old_makefile_cannot_be_reported_as_lto_enabled(self):
        plan = "gcc -c -Og -DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_TCOBS main.c -o out/main.o\n"
        plan += "gcc -Og -flto out/main.o -o out/F030_bare.elf\n"
        with self.assertRaisesRegex(ValueError, "requested -Og/LTO"):
            check_build_plan(plan, "F030_bare", arguments([]))

    def test_lto_link_must_keep_section_garbage_collection_effective(self):
        plan = "gcc -c -Og -flto -g -gdwarf-2 -ffunction-sections -fdata-sections -DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_TCOBS main.c -o out/main.o\n"
        plan += "gcc -Og -flto -g -gdwarf-2 -Wl,--gc-sections out/main.o -o out/F030_bare.elf\n"
        with self.assertRaisesRegex(ValueError, "preserve function/data sections"):
            check_build_plan(plan, "F030_bare", arguments([]))

    def test_lto_link_cannot_switch_to_another_dwarf_version(self):
        plan = "gcc -c -Og -flto -g -gdwarf-2 -ffunction-sections -fdata-sections -DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_TCOBS main.c -o out/main.o\n"
        plan += "gcc -Og -flto -g -ffunction-sections -fdata-sections out/main.o -o out/F030_bare.elf\n"
        with self.assertRaisesRegex(ValueError, "matching DWARF 2"):
            check_build_plan(plan, "F030_bare", arguments([]))

    def test_local_source_overlay_preserves_user_files_and_records_its_patch(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            repository, snapshot = root / "repository", root / "snapshot"
            relative = Path("examples/F030_bare/Makefile")
            for base in (repository, snapshot):
                (base / relative).parent.mkdir(parents=True)
            (repository / relative).write_text("current LTO settings\n", encoding="utf-8")
            (snapshot / relative).write_text("committed settings\n", encoding="utf-8")
            with patch("memory_usage.subprocess.check_output", side_effect=[b"source patch", str(relative).replace("\\", "/").encode() + b"\0"]):
                digest = overlay_sources(snapshot, repository)
            self.assertEqual((snapshot / relative).read_text(), "current LTO settings\n")
            self.assertEqual((repository / relative).read_text(), "current LTO settings\n")
            self.assertEqual(len(digest), 64)


if __name__ == "__main__":
    unittest.main()

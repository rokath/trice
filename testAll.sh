#!/usr/bin/env bash

    # test quick:          ./testAll.sh
    # test config compile: ./testAll.sh config
    # test full:           ./testAll.sh full

# === Define your long-running task ===
my_long_task() {
    #local arg1="${1:-default1}"
    #local arg2="${2:-default2}"

    echo "🟢 Starting Test: $(date)"
    #echo "Test parameters: arg1='$arg1', arg2='$arg2'"

    SELECTED=${1:-quick}

    triceFolder=`pwd`
    date                                                 2>&1 | tee    $triceFolder/testAll.log
    echo This can take a while ...                       2>&1 | tee -a $triceFolder/testAll.log
    t0=`date +%s`

    # show environment and prepare
    echo "SELECTED: $SELECTED"                           2>&1 | tee -a $triceFolder/testAll.log
    echo \$OSTYPE=$OSTYPE                                2>&1 | tee -a $triceFolder/testAll.log
    if command -v uname; then
        uname -a                                         2>&1 | tee -a $triceFolder/testAll.log
    fi
    if command -v go; then
        go version                                       2>&1 | tee -a $triceFolder/testAll.log
        go install ./...                                 2>&1 | tee -a $triceFolder/testAll.log
    fi
    which trice                                          2>&1 | tee -a $triceFolder/testAll.log
    trice version                                        2>&1 | tee -a $triceFolder/testAll.log
    ./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
    rm -f demoTIL.json demoLI.json                       2>&1 | tee -a $triceFolder/testAll.log
    touch demoTIL.json demoLI.json                       2>&1 | tee -a $triceFolder/testAll.log
    ./renewIDs_in_examples_and_refresh_test_folder.sh    2>&1 | tee -a $triceFolder/testAll.log


    if [ "$SELECTED" != "config" ]; then
        if command -v go; then
            # Go code tests
            echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
            echo "Testing the Go code..."                        2>&1 | tee -a $triceFolder/testAll.log
                go clean -cache -testcache                       2>&1 | tee -a $triceFolder/testAll.log
                go test ./...                                    2>&1 | tee -a $triceFolder/testAll.log
                rc=$?
                if [ $rc -ne 0 ]; then
                    echo "Testing the Go code...failed"               | tee -a $triceFolder/testAll.log
                    exit $rc
                fi
                if cat $triceFolder/testAll.log | grep -q FAIL ; then
                    echo "Testing the Go code...FAILed"               | tee -a $triceFolder/testAll.log
                    exit $rc
                fi
            echo "Testing the Go code...pass"                    2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log


            # Target code inside PC tests
            echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
            echo "Testing the Target code inside PC..."          2>&1 | tee -a $triceFolder/testAll.log
            ./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
                cd _test
                    if [ "$C_INCLUDE_PATH" != "" ]; then
                        echo It is important, that C_INLUDE_PATH is not set for the CGO tests. Clearing it temporarily.
                        export C_INCLUDE_PATH=""
                    fi
                    if [ "$SELECTED" = "quick" ]; then
                        echo "go test ./be_dblB_de_tcobs_ua/..."         2>&1 | tee -a $triceFolder/testAll.log
                        go test ./be_dblB_de_tcobs_ua/...                2>&1 | tee -a $triceFolder/testAll.log
                        rc=$?
                        if [ $rc -ne 0 ]; then
                            echo "Testing the Target code inside PC...failed" | tee -a $triceFolder/testAll.log
                            exit $rc
                        fi
                        # On build errors rc can be still 0, so we check the log file for FAIL
                        if cat $triceFolder/testAll.log | grep -q FAIL ; then
                            echo "Testing the Target code inside PC...failed"               | tee -a $triceFolder/testAll.log
                            echo "In case of CGO build errors check the PATH variable too." | tee -a $triceFolder/testAll.log
                            exit 2
                        fi
                    fi
                    if [ "$SELECTED" = "full" ]; then
                        echo "go test ./..."                             2>&1 | tee -a $triceFolder/testAll.log
                        go test ./...                                    2>&1 | tee -a $triceFolder/testAll.log
                        rc=$?
                        if [ $rc -ne 0 ]; then
                            echo "Testing the Target code inside PC...failed" | tee -a $triceFolder/testAll.log
                            exit $rc
                        fi
                        # On build errors rc can be still 0, so we check the log file for FAIL
                        if cat $triceFolder/testAll.log | grep -q FAIL ; then
                            echo "Testing the Target code inside PC...failed"               | tee -a $triceFolder/testAll.log
                            echo "In case of CGO build errors check the PATH variable too." | tee -a $triceFolder/testAll.log
                            exit 2
                        fi
                    fi
                cd - >/dev/null
            ./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
            echo "Testing the Target code inside PC...pass"      2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
        else
            echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
            echo "WARNING:     Go not installed."                2>&1 | tee -a $triceFolder/testAll.log
            echo "Skipping Go code and PC target code tests."    2>&1 | tee -a $triceFolder/testAll.log
            echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        fi
    fi

    # set build environment
    source ./build_environment.sh                            2>&1 | tee -a $triceFolder/testAll.log

    # clang translation test
    if ! command -v clang; then
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo "WARNING:     clang not installed"              2>&1 | tee -a $triceFolder/testAll.log
        echo "Skipping clang arm target G0B1 translation."   2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    else
        clang --version                                          2>&1 | tee -a $triceFolder/testAll.log
        if ! command -v arm-none-eabi-gcc; then
            echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
            echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
            echo "WARNING:    arm-none-eabi-gcc not installed"   2>&1 | tee -a $triceFolder/testAll.log
            echo "Skipping clang arm target G0B1 translation."   2>&1 | tee -a $triceFolder/testAll.log
            echo "The arm-none-eabi-gcc libraries are needed."   2>&1 | tee -a $triceFolder/testAll.log
            echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
            echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
        else
            echo "---"                                                   2>&1 | tee -a $triceFolder/testAll.log
            echo "Translating G0B1_inst with clang..."                   2>&1 | tee -a $triceFolder/testAll.log
                cd examples/G0B1_inst
                    make clean                                           2>&1 | tee -a $triceFolder/testAll.log 
                    ./build_with_clang.sh                                2>&1 | tee -a $triceFolder/testAll.log
                cd - >/dev/null
                if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                    echo "Translating G0B1_inst with clang...failed"          | tee -a $triceFolder/testAll.log
                    exit 2
                fi
            echo "Translating G0B1_inst with clang...pass"               2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                                   2>&1 | tee -a $triceFolder/testAll.log
        fi
    fi

    # gcc translation tests
    # We need the C_INCLUDE_PATH to point to the arm-none-eabi-gcc include files folder.
    if command -v  arm-none-eabi-gcc; then
        arm-none-eabi-gcc --version | grep gcc               2>&1 | tee -a $triceFolder/testAll.log
    fi
    echo "C_INCLUDE_PATH=$C_INCLUDE_PATH"                    2>&1 | tee -a $triceFolder/testAll.log
    echo arm-none-eabi-gcc location in next line:            2>&1 | tee -a $triceFolder/testAll.log
    which arm-none-eabi-gcc                                  2>&1 | tee -a $triceFolder/testAll.log
    if ! command -v arm-none-eabi-gcc; then
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo "WARNING:     arm-none-eabi-gcc not installed"  2>&1 | tee -a $triceFolder/testAll.log
        echo "skipping gcc arm target examples translation"
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    else
        # translate gcc arm target examples
        ./trice_insertIDs_in_examples_and_test_folder.sh                 2>&1 | tee -a $triceFolder/testAll.log
            cd examples
                ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
                echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
                echo "Translating all examples with TRICE_OFF..."        2>&1 | tee -a $triceFolder/testAll.log
                ./buildAllTargets_TRICE_OFF.sh                           2>&1 | tee -a $triceFolder/testAll.log
                if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                    echo "Translating all examples with TRICE_OFF...failed"   | tee -a $triceFolder/testAll.log
                    cd - >/dev/null
                    exit 2
                fi
                echo "Translating all examples with TRICE_OFF...pass"    2>&1 | tee -a $triceFolder/testAll.log
                echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
                ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
                echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
                echo "Translating all examples with TRICE_ON..."         2>&1 | tee -a $triceFolder/testAll.log
                ./buildAllTargets_TRICE_ON.sh                            2>&1 | tee -a $triceFolder/testAll.log
                if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                    echo "Translating all examples with TRICE_ON...failed"    | tee -a $triceFolder/testAll.log
                    cd - >/dev/null
                    exit 2
                fi
                echo "Translating all examples with TRICE_ON...pass"     2>&1 | tee -a $triceFolder/testAll.log
                echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
                ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
            cd - >/dev/null
            if [ $SELECTED = "full" ] || [ $SELECTED = "config" ]; then
                cd examples/L432_inst
                    echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
                    echo "Translating all L432 configurations..."        2>&1 | tee -a $triceFolder/testAll.log
                    ./all_configs_build.sh                               2>&1 | tee -a $triceFolder/testAll.log
                    if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                        echo "Translating all L432 configurations...failed"   | tee -a $triceFolder/testAll.log
                        cd - >/dev/null
                        exit 2
                    fi
                    echo "Translating all L432 configurations...pass"    2>&1 | tee -a $triceFolder/testAll.log
                    echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
                cd - >/dev/null
            fi
        ./trice_cleanIDs_in_examples_and_test_folder.sh                  2>&1 | tee -a $triceFolder/testAll.log
    fi

    t1=`date +%s`
    runtime=$((t1-t0))
    echo Script run $runtime seconds.                    2>&1 | tee -a $triceFolder/testAll.log
    echo "✅ Test completed at: $(date)"
}

# === Inject the function and arguments into subshell ===
get_function_with_call() {
    declare -f my_long_task
    printf "my_long_task %q %q\n" "$1" "$2"
}

# === Linux: systemd-inhibit ===
run_with_inhibit_linux() {
    echo "🧪 Detected Linux."
    if command -v systemd-inhibit >/dev/null; then
        echo "🔒 Using systemd-inhibit..."
        systemd-inhibit --what=sleep:shutdown --who="$(whoami)" --why="Running long task" --mode=block \
            bash -c "$(get_function_with_call "$1" "$2")"
    else
        echo "⚠️ systemd-inhibit not found. Running without inhibition."
        my_long_task "$1" "$2"
    fi
}

# === macOS: caffeinate ===
run_with_inhibit_macos() {
    echo "🍎 Detected macOS."
    if command -v caffeinate >/dev/null; then
        echo "🔒 Using caffeinate..."
        caffeinate -dimsu zsh -c "$(get_function_with_call "$1" "$2")"
    else
        echo "⚠️ caffeinate not found. Running without inhibition."
        my_long_task "$1" "$2"
    fi
}

# === Windows (Git Bash): PowerShell inhibit ===
run_with_inhibit_windows() {
    echo "🪟 Detected Windows (Git Bash). Using PowerShell inhibit."

    powershell -Command "
    Add-Type -Namespace Sleep -Name Preventer -MemberDefinition '
        [DllImport(\"kernel32.dll\", SetLastError=true)]
        public static extern uint SetThreadExecutionState(uint esFlags);
    ';
    [Sleep.Preventer]::SetThreadExecutionState(0x80000000 -bor 0x00000001);
    " > /dev/null

    TMP_SCRIPT="/tmp/my_task_win.sh"
    get_function_with_call "$1" "$2" > "$TMP_SCRIPT"
    chmod +x "$TMP_SCRIPT"
    bash "$TMP_SCRIPT"

    powershell -Command "[Sleep.Preventer]::SetThreadExecutionState(0x80000000);" > /dev/null
}

# === Fallback for unknown systems ===
run_without_inhibit() {
    echo "⚠️ Unsupported OS. Running without sleep prevention."
    my_long_task "$1" "$2"
}

# === Main dispatcher ===
OS_TYPE="$(uname -s)"
case "$OS_TYPE" in
    Linux*)    run_with_inhibit_linux "$@" ;;
    Darwin*)   run_with_inhibit_macos "$@" ;;
    MINGW*|MSYS*|CYGWIN*) run_with_inhibit_windows "$@" ;;
    *)         run_without_inhibit "$@" ;;
esac

echo "🏁 Done."

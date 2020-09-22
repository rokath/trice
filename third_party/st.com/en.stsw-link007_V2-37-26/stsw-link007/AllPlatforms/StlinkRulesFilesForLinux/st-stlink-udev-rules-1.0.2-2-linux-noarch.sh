#!/bin/sh
# This script was generated using Makeself 2.2.0

umask 077

CRCsum="1679344230"
MD5="f3c4a3a4aae65f88a4a3bee165d42306"
TMPROOT=${TMPDIR:=/tmp}

label="STM udev rules installer"
script="./setup.sh"
scriptargs="1.0.2-2"
licensetxt=""
targetdir="root"
filesizes="6673"
keep="y"
quiet="n"

print_cmd_arg=""
if type printf > /dev/null; then
    print_cmd="printf"
elif test -x /usr/ucb/echo; then
    print_cmd="/usr/ucb/echo"
else
    print_cmd="echo"
fi

unset CDPATH

MS_Printf()
{
    $print_cmd $print_cmd_arg "$1"
}

MS_PrintLicense()
{
  if test x"$licensetxt" != x; then
    echo $licensetxt
    while true
    do
      MS_Printf "Please type y to accept, n otherwise: "
      read yn
      if test x"$yn" = xn; then
        keep=n
 	eval $finish; exit 1        
        break;    
      elif test x"$yn" = xy; then
        break;
      fi
    done
  fi
}

MS_diskspace()
{
	(
	if test -d /usr/xpg4/bin; then
		PATH=/usr/xpg4/bin:$PATH
	fi
	df -kP "$1" | tail -1 | awk '{ if ($4 ~ /%/) {print $3} else {print $4} }'
	)
}

MS_dd()
{
    blocks=`expr $3 / 1024`
    bytes=`expr $3 % 1024`
    dd if="$1" ibs=$2 skip=1 obs=1024 conv=sync 2> /dev/null | \
    { test $blocks -gt 0 && dd ibs=1024 obs=1024 count=$blocks ; \
      test $bytes  -gt 0 && dd ibs=1 obs=1024 count=$bytes ; } 2> /dev/null
}

MS_dd_Progress()
{
    if test "$noprogress" = "y"; then
        MS_dd $@
        return $?
    fi
    file="$1"
    offset=$2
    length=$3
    pos=0
    bsize=4194304
    while test $bsize -gt $length; do
        bsize=`expr $bsize / 4`
    done
    blocks=`expr $length / $bsize`
    bytes=`expr $length % $bsize`
    (
        dd bs=$offset count=0 skip=1 2>/dev/null
        pos=`expr $pos \+ $bsize`
        MS_Printf "     0%% " 1>&2
        if test $blocks -gt 0; then
            while test $pos -le $length; do
                dd bs=$bsize count=1 2>/dev/null
                pcent=`expr $length / 100`
                pcent=`expr $pos / $pcent`
                if test $pcent -lt 100; then
                    MS_Printf "\b\b\b\b\b\b\b" 1>&2
                    if test $pcent -lt 10; then
                        MS_Printf "    $pcent%% " 1>&2
                    else
                        MS_Printf "   $pcent%% " 1>&2
                    fi
                fi
                pos=`expr $pos \+ $bsize`
            done
        fi
        if test $bytes -gt 0; then
            dd bs=$bytes count=1 2>/dev/null
        fi
        MS_Printf "\b\b\b\b\b\b\b" 1>&2
        MS_Printf " 100%%  " 1>&2
    ) < "$file"
}

MS_Help()
{
    cat << EOH >&2
Makeself version 2.2.0
 1) Getting help or info about $0 :
  $0 --help   Print this message
  $0 --info   Print embedded info : title, default target directory, embedded script ...
  $0 --lsm    Print embedded lsm entry (or no LSM)
  $0 --list   Print the list of files in the archive
  $0 --check  Checks integrity of the archive
 
 2) Running $0 :
  $0 [options] [--] [additional arguments to embedded script]
  with following options (in that order)
  --confirm             Ask before running embedded script
  --quiet		Do not print anything except error messages
  --noexec              Do not run embedded script
  --keep                Do not erase target directory after running
			the embedded script
  --noprogress          Do not show the progress during the decompression
  --nox11               Do not spawn an xterm
  --nochown             Do not give the extracted files to the current user
  --target dir          Extract directly to a target directory
                        directory path can be either absolute or relative
  --tar arg1 [arg2 ...] Access the contents of the archive through the tar command
  --                    Following arguments will be passed to the embedded script
EOH
}

MS_Check()
{
    OLD_PATH="$PATH"
    PATH=${GUESS_MD5_PATH:-"$OLD_PATH:/bin:/usr/bin:/sbin:/usr/local/ssl/bin:/usr/local/bin:/opt/openssl/bin"}
	MD5_ARG=""
    MD5_PATH=`exec <&- 2>&-; which md5sum || type md5sum`
    test -x "$MD5_PATH" || MD5_PATH=`exec <&- 2>&-; which md5 || type md5`
	test -x "$MD5_PATH" || MD5_PATH=`exec <&- 2>&-; which digest || type digest`
    PATH="$OLD_PATH"

    if test "$quiet" = "n";then
    	MS_Printf "Verifying archive integrity..."
    fi
    offset=`head -n 500 "$1" | wc -c | tr -d " "`
    verb=$2
    i=1
    for s in $filesizes
    do
		crc=`echo $CRCsum | cut -d" " -f$i`
		if test -x "$MD5_PATH"; then
			if test `basename $MD5_PATH` = digest; then
				MD5_ARG="-a md5"
			fi
			md5=`echo $MD5 | cut -d" " -f$i`
			if test $md5 = "00000000000000000000000000000000"; then
				test x$verb = xy && echo " $1 does not contain an embedded MD5 checksum." >&2
			else
				md5sum=`MS_dd "$1" $offset $s | eval "$MD5_PATH $MD5_ARG" | cut -b-32`;
				if test "$md5sum" != "$md5"; then
					echo "Error in MD5 checksums: $md5sum is different from $md5" >&2
					exit 2
				else
					test x$verb = xy && MS_Printf " MD5 checksums are OK." >&2
				fi
				crc="0000000000"; verb=n
			fi
		fi
		if test $crc = "0000000000"; then
			test x$verb = xy && echo " $1 does not contain a CRC checksum." >&2
		else
			sum1=`MS_dd "$1" $offset $s | CMD_ENV=xpg4 cksum | awk '{print $1}'`
			if test "$sum1" = "$crc"; then
				test x$verb = xy && MS_Printf " CRC checksums are OK." >&2
			else
				echo "Error in checksums: $sum1 is different from $crc" >&2
				exit 2;
			fi
		fi
		i=`expr $i + 1`
		offset=`expr $offset + $s`
    done
    if test "$quiet" = "n";then
    	echo " All good."
    fi
}

UnTAR()
{
    if test "$quiet" = "n"; then
    	tar $1vf - 2>&1 || { echo Extraction failed. > /dev/tty; kill -15 $$; }
    else

    	tar $1f - 2>&1 || { echo Extraction failed. > /dev/tty; kill -15 $$; }
    fi
}

finish=true
xterm_loop=
noprogress=n
nox11=n
copy=none
ownership=y
verbose=n

initargs="$@"

while true
do
    case "$1" in
    -h | --help)
	MS_Help
	exit 0
	;;
    -q | --quiet)
	quiet=y
	noprogress=y
	shift
	;;
    --info)
	echo Identification: "$label"
	echo Target directory: "$targetdir"
	echo Uncompressed size: 16 KB
	echo Compression: gzip
	echo Date of packaging: Tue Sep 24 11:44:18 UTC 2019
	echo Built with Makeself version 2.2.0 on 
	echo Build command was: "/usr/bin/makeself \\
    \"--notemp\" \\
    \"/src/work/udev_rules_installer/makeself/root\" \\
    \"/src/work/udev_rules_installer/makeself/st-stlink-udev-rules-1.0.2-2-linux-noarch.sh\" \\
    \"STM udev rules installer\" \\
    \"./setup.sh\" \\
    \"1.0.2-2\""
	if test x$script != x; then
	    echo Script run after extraction:
	    echo "    " $script $scriptargs
	fi
	if test x"" = xcopy; then
		echo "Archive will copy itself to a temporary location"
	fi
	if test x"y" = xy; then
	    echo "directory $targetdir is permanent"
	else
	    echo "$targetdir will be removed after extraction"
	fi
	exit 0
	;;
    --dumpconf)
	echo LABEL=\"$label\"
	echo SCRIPT=\"$script\"
	echo SCRIPTARGS=\"$scriptargs\"
	echo archdirname=\"root\"
	echo KEEP=y
	echo COMPRESS=gzip
	echo filesizes=\"$filesizes\"
	echo CRCsum=\"$CRCsum\"
	echo MD5sum=\"$MD5\"
	echo OLDUSIZE=16
	echo OLDSKIP=501
	exit 0
	;;
    --lsm)
cat << EOLSM
No LSM.
EOLSM
	exit 0
	;;
    --list)
	echo Target directory: $targetdir
	offset=`head -n 500 "$0" | wc -c | tr -d " "`
	for s in $filesizes
	do
	    MS_dd "$0" $offset $s | eval "gzip -cd" | UnTAR t
	    offset=`expr $offset + $s`
	done
	exit 0
	;;
	--tar)
	offset=`head -n 500 "$0" | wc -c | tr -d " "`
	arg1="$2"
    if ! shift 2; then MS_Help; exit 1; fi
	for s in $filesizes
	do
	    MS_dd "$0" $offset $s | eval "gzip -cd" | tar "$arg1" - $*
	    offset=`expr $offset + $s`
	done
	exit 0
	;;
    --check)
	MS_Check "$0" y
	exit 0
	;;
    --confirm)
	verbose=y
	shift
	;;
	--noexec)
	script=""
	shift
	;;
    --keep)
	keep=y
	shift
	;;
    --target)
	keep=y
	targetdir=${2:-.}
    if ! shift 2; then MS_Help; exit 1; fi
	;;
    --noprogress)
	noprogress=y
	shift
	;;
    --nox11)
	nox11=y
	shift
	;;
    --nochown)
	ownership=n
	shift
	;;
    --xwin)
	finish="echo Press Return to close this window...; read junk"
	xterm_loop=1
	shift
	;;
    --phase2)
	copy=phase2
	shift
	;;
    --)
	shift
	break ;;
    -*)
	echo Unrecognized flag : "$1" >&2
	MS_Help
	exit 1
	;;
    *)
	break ;;
    esac
done

if test "$quiet" = "y" -a "$verbose" = "y";then
	echo Cannot be verbose and quiet at the same time. >&2
	exit 1
fi

MS_PrintLicense

case "$copy" in
copy)
    tmpdir=$TMPROOT/makeself.$RANDOM.`date +"%y%m%d%H%M%S"`.$$
    mkdir "$tmpdir" || {
	echo "Could not create temporary directory $tmpdir" >&2
	exit 1
    }
    SCRIPT_COPY="$tmpdir/makeself"
    echo "Copying to a temporary location..." >&2
    cp "$0" "$SCRIPT_COPY"
    chmod +x "$SCRIPT_COPY"
    cd "$TMPROOT"
    exec "$SCRIPT_COPY" --phase2 -- $initargs
    ;;
phase2)
    finish="$finish ; rm -rf `dirname $0`"
    ;;
esac

if test "$nox11" = "n"; then
    if tty -s; then                 # Do we have a terminal?
	:
    else
        if test x"$DISPLAY" != x -a x"$xterm_loop" = x; then  # No, but do we have X?
            if xset q > /dev/null 2>&1; then # Check for valid DISPLAY variable
                GUESS_XTERMS="xterm rxvt dtterm eterm Eterm kvt konsole aterm"
                for a in $GUESS_XTERMS; do
                    if type $a >/dev/null 2>&1; then
                        XTERM=$a
                        break
                    fi
                done
                chmod a+x $0 || echo Please add execution rights on $0
                if test `echo "$0" | cut -c1` = "/"; then # Spawn a terminal!
                    exec $XTERM -title "$label" -e "$0" --xwin "$initargs"
                else
                    exec $XTERM -title "$label" -e "./$0" --xwin "$initargs"
                fi
            fi
        fi
    fi
fi

if test "$targetdir" = "."; then
    tmpdir="."
else
    if test "$keep" = y; then
	if test "$quiet" = "n";then
	    echo "Creating directory $targetdir" >&2
	fi
	tmpdir="$targetdir"
	dashp="-p"
    else
	tmpdir="$TMPROOT/selfgz$$$RANDOM"
	dashp=""
    fi
    mkdir $dashp $tmpdir || {
	echo 'Cannot create target directory' $tmpdir >&2
	echo 'You should try option --target dir' >&2
	eval $finish
	exit 1
    }
fi

location="`pwd`"
if test x$SETUP_NOCHECK != x1; then
    MS_Check "$0"
fi
offset=`head -n 500 "$0" | wc -c | tr -d " "`

if test x"$verbose" = xy; then
	MS_Printf "About to extract 16 KB in $tmpdir ... Proceed ? [Y/n] "
	read yn
	if test x"$yn" = xn; then
		eval $finish; exit 1
	fi
fi

if test "$quiet" = "n";then
	MS_Printf "Uncompressing $label"
fi
res=3
if test "$keep" = n; then
    trap 'echo Signal caught, cleaning up >&2; cd $TMPROOT; /bin/rm -rf $tmpdir; eval $finish; exit 15' 1 2 3 15
fi

leftspace=`MS_diskspace $tmpdir`
if test -n "$leftspace"; then
    if test "$leftspace" -lt 16; then
        echo
        echo "Not enough space left in "`dirname $tmpdir`" ($leftspace KB) to decompress $0 (16 KB)" >&2
        if test "$keep" = n; then
            echo "Consider setting TMPDIR to a directory with more free space."
        fi
        eval $finish; exit 1
    fi
fi

for s in $filesizes
do
    if MS_dd_Progress "$0" $offset $s | eval "gzip -cd" | ( cd "$tmpdir"; UnTAR x ) 1>/dev/null; then
		if test x"$ownership" = xy; then
			(PATH=/usr/xpg4/bin:$PATH; cd "$tmpdir"; chown -R `id -u` .;  chgrp -R `id -g` .)
		fi
    else
		echo >&2
		echo "Unable to decompress $0" >&2
		eval $finish; exit 1
    fi
    offset=`expr $offset + $s`
done
if test "$quiet" = "n";then
	echo
fi

cd "$tmpdir"
res=0
if test x"$script" != x; then
    if test x"$verbose" = xy; then
		MS_Printf "OK to execute: $script $scriptargs $* ? [Y/n] "
		read yn
		if test x"$yn" = x -o x"$yn" = xy -o x"$yn" = xY; then
			eval $script $scriptargs $*; res=$?;
		fi
    else
		eval $script $scriptargs $*; res=$?
    fi
    if test $res -ne 0; then
		test x"$verbose" = xy && echo "The program '$script' returned an error code ($res)" >&2
    fi
fi
if test "$keep" = n; then
    cd $TMPROOT
    /bin/rm -rf $tmpdir
fi
eval $finish; exit $res
‹ ’Š]íTgP”k“2Y	"CNC$Ã%'Éa€™!‹d9#ˆ$Aœ³€d$KÉY$Š  ²èŞ»{ï·ûıÙªo«vËSï[óV÷yúéîÓÓ¼|€9ø¯qWDÀ/ pWXäîõ¯à]Ñ»Â¿ì   $",(ÌÏ/*(xíç€D ÿğ@¡­‘ À„Â­ÿ9ÏÁÕÆÚMğÿ¼|((ÚÃåø¯Ö_àŸé/ r­ùßõ¸¦ƒøëÿ/3#ŸÌ•ÏÆåzB‘(ÂUŠE „ÙƒLA<¾ &–?¬L s$íu@m i6A“…‚¹:€ş ¬‘¼ 9ıÓ
s½î.n¾vñ2]Ÿó†¡A@{ˆô€CQ–nÖhG)>(Ú–ÏÃêÉ÷ËÈkÇ',Î¼K“‡«3ˆÅï?¹ş\@”#Ì}‰B[ştü%[ûëlÿÓñ×„™Aªö??‘Ğë¯_8jmçóg~P;=ìú
0ÈÖjë²G A×7ˆCOßR[]ÙÒPIWOUë>çÏP
¿H×çş¬uÈîa÷³äëK@W( Ù@ÿrµ«…‚¼a¶ êñº3PÄºîˆÇã!Úñ\ßòïËÿP„H@ğóı÷Öÿ©ÉOƒµ—3ˆb7™¡¥Ì¹ÙAì|Ï™ÏÏ	sEƒX„üÙÿ±™œ ‡Ünzú# 'ğg¥Ï„rDxÀí~Ö…ğ@‚l¯K‚Ù]3~õë¯R	~éÁòÇ¥ş6Mjó—iº@ƒ×/$ı×©úÇH^0´ãuGA¸ù«İædúñçœñ	®íç¬1ƒ 0×ëä|@vˆ_}ÿ[²?ÿÿ¾Şö×Šó Ğp˜«3ÏÏYäùÕ'.k‹‡7Ï5›÷šÈëà¼^™ÿ“÷˜Àÿsûÿ¿+[€—ŸWGğ¿ÿ?Şÿ¢ÂÂÿdÿ	‰
ı}ÿò		şŞÿÿ`ˆÀÄc„›°vÍœcé&<¨Èë§	bc2İhD‹Yìíä!•,eÕÌ0«šÙÙÊOƒ3ö$…Rv·éÏ+^-“Ç…âq„¹‹0v¾"}…mVI(®Šsƒ‹õ Ç'
w'ÂtW‰t¡ïbŞL÷Ch}.Å·Ko¨ÍİŞ›;Í8=ÚvıZÅ«uÛÌîÚ²pDÉpÓÉğÃıºn;O•Bù×ß‹ş‘' ,\í‰îé]5Û‹_^¦„;¢¿ÅS‚½rğBÛ
ñr½KiÁ"ˆ††Q÷eœÖe¢ó‰Î°bĞ„+v»Çs„o}yå@kDT¹ÏŞ™‚ãE?‰Ãòçp|‚)ø8LÏúl…â…à€ø«ö™ûA÷DeÒî@‹
å´O§HI&CMDêwÏˆ‡BàJØå‰”{ ²)ÃàV“|œÒ¹uŸ=cY3wû|~©©û¥†hşÃøíÉúX¢é´çv›‰àyúW™ÄAŒ$›—›»X7½KÄ„0æ?&o»Œ’<tÇrxåFvN9ÄÊ«¡5øyZß—ûşSys-¼‹$Š—áBíaşWQŞ«ƒÏV¢¿wvBJˆÆÄÕÍi% Q–/lÚU£h-ÄœÙÏÇ•¬+4¶
ŠüU¨ÍvæÊ1~´›ø,n,ŞòµLsíu²úTì4€²7YÆ—9p3>íl.o0À¨ÁåÕ[.<HİOy¼|æ­·Ä†«ì¾óïĞ)Ï.İ˜¤Ã’cŠ=&-¸Ì©«İö³4
«‚í}	éÈd.2l‚VÍ„ŞKU¸¬r§w#íìH%HlFÖºõ1=Lçy§|©ó‘´mò4õ1rRÛWLª2m¾“‹¾L(Áå6öZ~n`æNim±
èeØèÆ®ó/½_¨‡u1¤h‚Î·“) úaĞIF[ÁÇE9‰tTP©Ğèmtˆ
Â$n/EGq6óTµJÜÇ¸sşz¼-¡·îù’Ñè¨‚ªòGR%6ÅåéF+ôsxR•’)Ñ—eCéêÈKrÁ
Né3ík$Ó	¤mlÏš×};¡É>Ö!Q5ˆS‘hòÕ7Bug|şÊäŠÃ=roÎ`¹Ô‚À7*¢ìŠm¼æä(—ùÌ	ğü(ÿ„ıbŒ„ı÷p+ãw®ÍÛV²Œ‹SƒçwÁ“k-!ujuŸòÖı5µñÓ[OWÎ¸-ñv¸ßÍ£¾…¤—©Gh®0œWDl(‘ä±•Æ9Äp¾X}òÆŸ®'~¾jÿªáÃ%x
ÃgÊ¬¬é R-¬‡›‚.¬v. e™ƒ|yÅYôËî¤JşyÈl5n“ÚÈªˆWFx¯pFûÇO³ÉµÊ7´¦°ïàh?Å÷ïifY”µQM“ÌâïúºE¾ó]­1L…nLƒT8¦Û¤ß]•víÄNåô4ÙBfR¸Uvû¿ZÅ>·Åº+Ï†Äœ”©>±Oÿ4cıidi0k4œHëe‰šy†TÒ¶ÊAôèÛŸ°Tú˜Yúk00§ÔójPŸŸ5Œ÷L«Dì®Lš¿4s¨÷ˆ}kkùÎ`|Ö‹W~”P°—IšÌÉ«|èĞš‹'Û÷(Ù}ûâIGí_×ô±5òÂWtçë ”·Šé©ı mûë±d x{Š8a.Kw÷Æw—M¢%Õ')9ÄÏ,ßºC¡\mA¹B\}i¡’«“¸¸mOà»|w)§”ï~SÜq“Õ{/ıjÂ‰­±Éù$¡`)Ö_Uè%÷IPÊskã•½àB¥½¯Ï’Ç5ˆÅ9Ş»ƒ^é[g,]UŸPqdŒº¶û*·x™±¯İˆH
ã)-x•XáÎ…kÕ¬Ba}Ò£/Ã¡÷VDÌ÷<à‹‚	uršPºøk§¡–Nİ*Èø³ŠRÁIo+09§cÀ 4[8Ø³ÙPÕ¤Ë"-oÊÒ&ßºb\€)èxlïµôé.~Ü|½!€9Jdë*bdí‰,MÜœÚ cMÍ!X¦öï'c:ì~t¬™]e°
™ÖÍò‚ÆBÏ7'Súâñªˆµ°é´x¬/mìê\ÌYwµË&\Çk¦²0B £ó
ënÖôpo­\..›¹Î³Ş¹\üèVvå'qŸª¥ˆúˆêL§e‚7f¦O¶>rx„dKUa«6¸›< €Q¿QÕÃìÜÃ§ÜÔé#¾‘”VÁµàVl×¾£-ÕEµ­—'VÛÓz1và”å_HÖB³oâÓŞgãx«Ë µ3VÁ^0öt™s!3HÃÌõÛ[‘¸ÊçØå ˜1²Õ­ºI°êpÉCÛØRãy8;¾îz©³QÁŸÑ#¸5¨ìMl3¼%"®p7ñ½çµêÔIqë(_{­Ä÷ïÓÌ‰‹Äñ½?.í?2•3òMH±Ñöç‰Q¸õ˜<E¤S-È¶Jq‘³˜e&éTl¬«LU‡H,†½¢		$ÕhÌÛ)' HÀ% 
ËxV¢JÊ¨ú:CnĞ1ÙÛPŠ’ıf|ŞÛi—‡=¤ŸMhbßn’òÏozyHòÌG€ÀCJ·Ki0bgÉê¾`¶š—íùBdçI_Øn’Q­\XG“Şu|wŠ"Â/Úy¡k±t)»Ø\NæF?p´6ôÈÑ°)•ˆÌÆ]k Ò·ãëÆ÷®””\7¥áT±ÙÄ’3ÓìÇm-b¼Ò¤ıß@ì`Z{²œ×mcd{£«d‚Âëƒ¦_Û}Šh†óÂ…ŞL¯ĞÜ_R§fS¡ÙM‡4ºa˜å'4S´™µHÅÏB’@V¦Ê
ò5 ŞW|sŠ>ó!:GÖÚRîXN7ö¸‡Š»¥ä"ó"«eÌ$ˆg3¼4¿¹Üö £¨SÒÌâÜY8X‘¦­·Bu:ÅĞÍ:Ü¡#V[óhn|îšè Ş“}ÂôbkH’+zB_R5á˜¦OQgí…¿fÄÖø¾&æmìÂ*kı|¾µS¹XÃ¶ÒvRmvMYşûÍpcòÆ9$Ìá™•’ûƒd1¬HœXÚÙÇÚ±r_ÿ.ƒ'djæ|Ú$ª2«'˜QÃ¯	ŞÊõãÈÆÄ}|^L5T`;Ï#+c·ŞTú|@‹Ú‹âõôôšØ\£³Ó>Úú‚ä8†¼ø¶h1/åiä-Ï\’‹˜¦u;Ìò*Œw±çºhî`èĞ} 3Ïzò?˜!gŠ“ã“ƒ´…Ü”&ÛûSS¦%ÀÌã±q}0Õ½µèÄÃx‚z"{úy2ñËä&rœq®^˜J–õí‡îöå%&Û3<FxÁ·°¥0À”[Rï··E·UBbã°VCë0±‡şp»÷¡˜=À.¦–‚‰z`Ç$„$#4­(g^/dÄØ¢òp#}UtkÆP¶ …T´!ê/àÆÊæ/P£=&»7®‘ğL	ç³oõío…6&Ş¿Ïa¢“Rgkzg8RMEøõqˆ½âH¢|áaœÈ¤îG,ìõÀq~=X3å¶â›Ësmï§n²Ü šzn`…]W¦F£Af±Ò;©ubV,’6Kjº<SŠ[¬ A¥AÁ3ÜØÙ$*ñ&
0f© ê¼¶&ä˜¢}Ry¢ªº^Öù®¾p¤}Y¨·yß³x_`[ÄK…$°c´[‹õ¦w71î©u#<‡:›w3”ÈN…¡8øµ’ñèûBüQzz2G‹¶DÍ92‘Y•uF™¼%ÔÎ_‚ªå%A§Tæ‰Ô”LN%aú9ÌJ€rÑ[Ó)øt‰\3$Aü
s€ºQŒ¾n8_º 'Œ­<ÑÓÍ„ÌëiôzCqlh¤SgÙ:ˆMÉ•kÔÎxR“šİ»=:6sÃ˜`ï“£Ô·0Úå»·¥†Ş¸Ó÷æ—¿3#cÈÙQa›­z”®²œÍB·Şí5Í ÄiÛ’+ÓÒæ¬ì2,8Ùóçw³ÌÚNî‹‚^Q›Ë…ä@ë+Ø´Æ™3£•ø†zµuz¼oãĞ4Ş43–õF´bÜLˆñT“Èò¹e¡¾ÕŞ¨ ™¨Nj ÃÉ	tˆ#I+økğ.­ş¡Õ—B¹ó)-ª›šSŒ·ñ§–&°Ày&Ôl	“Z#‹3p@EünÉ¤Õv}äC­7a6Je$AİòåéöS!Ë6YØOÚ¨9gh‡eYn¾WxÅ†¬¹[áe(Îêpˆ<3juâPÅŸ2lu<–’oÎ5“Ñ’KËÑ±İ˜D]W[À&RÁs±,Ü<E&¶>+’ñG §tüÚb¬%‹¨zÙlÊ8NR*Çğƒ½1bˆ}™ŞªÜì´¯Î% |Hw'·P€Í“&ÕJ–M–ËP"°¦1WA÷³ÅSÑ!œ“(ãĞ*'yµL°ÎÈQã†~ O™S†óF÷…,Èf0€æşmkCêC²PqVàCŠ:LjÃH7Eh‹Æò^?ş“­àµ}7DH1cFlYÛÓ@»œñyÊİ¡<'¬R@OX”XÏ4.ûWZòi®0÷Y¼¾H©$K¶äş|öÊsİ„>Gü¶Vvg†—œÉ¬Ojt°²<U‘•ğ'sVky©@Yrìïœü´¡Ô£3ìY Ì0KeyÙšWÜ}+ä™Ğƒâiº,/Òû¹É™Ö}îo6–öÆÈ×wÉÃ]Dî4Ö¾¤xñH¹ÍBğ&VÂó“^ôj=Ó‚ÇØkI›Ûö!Ó¾dl}p¥©ÓUhy¶_föñ…ÜYbIB![cÆró¹iEk¸ºu¿^/uU±lF2› ví\AäëÃtÖ±’öÂ*Ê¬-Ö1mÍ—ÌÓìTş Œ§OÉo(Âİ„%£¶ºÉgÀ¯ìõ½5ğÏ;ñGQ’D¡‹dõ0ƒî³-@ö’¡6[44©RMÁø`÷yëîyW	†ûYÉMıÌ°f\A€]ºpI	B@)*Û-ñVßlêhÕúÅ›à*ÔÃšnú¾PÅeŞ0_#²‚'d	“+úriFDN¥ß¸ìéÔ'Éë¨ßäH^•Néœ1Ni$&%`>£‹¸™'˜8ÇÖfı‚+@òÎ€ï3·«ü3Öİ¨vqsˆ¡cõøyôw³nlïi<]r+“),|HKJtä˜°¾ºI·È1;\•t	¯ÎLB°ë6V4z¿¦iH÷.	Ö·ié®å¬Ùù$êôªˆrLQA]ĞÑÜsùÂ«Ù Ûv>³@~c)õûñP;Kƒ@6¦†İ®uu•)ƒAD¡	`(«öxÙ‡éàÂÉÊüèÁûp$â‡’©¹!{Œƒ
%aBx H8%·c)nì\°¼‰mà´ŞéCkåŒÄíºh–º‹N#[Ì@èRP_g¯“V‘PVğ©Ä‚.ì”¨\§ oÆX\{´µx#ç­ê±ğ^RÙ‡5Â}¡‡ßl4W½o"å¦Ä¢ØpÄCS¼i÷YØ•±´?rä¼‹ÔÔÔ¤¨vIí­zVÁ]@à5ó-ÆÛ¤Dñ};à		³L‘m€ú³Zîà^ájkr¶mbôÊBH?®øQ|»•š`jF9Rzœ•íè,!KÅ¢1©C=V€ó}l“¢µŒzíXHçÆ áãrºJ b{DZi2°?¤ÕŸXKí?©¾Õøõ ğ‚—'Tn¹¡3ÈèÔ}Jv$Oÿƒ2şèék¬Lé>½`ÃÛ˜û¸ û_¬û+ÑÄÂT¯<™¤å–ağ=èÛ—¯/d9vî+¹ëî¯ì¯Ç;Áo;*êòÎ‚$/”;Êw%Øv=v©âGÊnqçİÆ•“oZ“aªgŞÕÉKh;qpm””wâã$z›M¢_ÍÎ•–tì<‰bQ|I©nµiŠã¦qœlSQîƒ¡¤<Â¤îÑy2NÕLæÒ6¶OŸÃÉõ˜¢Œ@g5q÷Ã<Ğ·ĞS]“”<Y'aÌ×m©;´I-YŒ'ÈZuU%çN?kÑÇZÃ„z³9D%#˜»u·º™Üß¸Ë:KCç0‹Â·­\ùÔü1ˆ »eaØ¥N–ZÚj¯6±ï;x®ÍŒOW+æ7~ş]ïf[ï¯®¾OhSÃsåÂ6Œ”ÌCŸÈÖ£IÜï`~°“±MßµÆúîùÂLú–™•Ïˆ,^¶²b<Hßä§²‘d!$?‡úñ'F§/YõÎ/0Ïm•ÌDVƒûu»°ÊGÚ	ÏÅã¸—såh!ÅÛÇ—°rı
-¶¢H%©|ŒÊØ?Í„.\õ­?æ3©[ºÌŒóPpöÛ¿J¬Ğl•	×"hjå__ƒs¢ƒcKÔÎ²qP;pMOÍÍññOpLIŸs‚4_‘Iù$+¡L’=úàö9]€çjoãèè~İöAùíä¹cOà3‡_×‚s(So†SÅe{$ë¥”µøòÒËu->n™äÊ²©tóŠİ¶³õêõ~? ÷XúòÇÕù¶×ñÂ{ÕGßN.cõú.úg®í·ğïß?Ñª}{Â¯XR1MhĞsU!*Å¼ã_Z"32’INê›E5•°^vTy¤uwöHåÑ³iìOGâ:FxJçõ³{ç:äC†.ÎIıÃ•äù¡¾–º»õ2øáÂ˜TÀ–äíä‰ò§Æ¥?ú‰ù¾ï_E®MF,öİÚd¸‡ê¢¾ñq‚Û°ôŞDîq7ç­ˆ‡Ø>×Üî«'Hæ†ûWû—§­÷ß;]Å´ küŒöÒM[ÏV(ıì÷šQi>“¤•ÊÒûÍ~®»Zé¨ı›—ù2—ˆ—g=ˆ=cÉ‹=çÉEGÃÑûÒ«,}Wm‡é¥®¶^dJœ—M4•©/yeîšûìëØ\”ğóZß_üÖm½9#3ÿ>ª*ú8ól`îêb=ŒuyG{ŞTUv¯ê`ûõlJÉ·¨Ö‡eóïã¨IO3Ïç3//P–~Ÿ“¿ôş(w:ïÏlº84%AˆßõéÑdÄîšÌß·ìØ,}¸Ü’¢ÅÎĞès”ğãxo£Í¿5`íd"éÅ^áwÕË™…€¥¯™ocIÊ–eæ¬$Q+É2¶~?&´ŸUH!¤>ãIL¾ãûè>ê¿3$$#tÜ¦éUÜ´y¦óèG‹ÔæÛØ†/û‰Qw¯¾uˆ®fø|Ë™ZP3»z¢Ewna,‹Ûdğ¹ä8<lı~Ä …<•iõ>S_\¸LÙò;å:Mê!iõXË“ªØü8<ü)ÁtáTbyÇøìÓ<é/æ§/›&cH›SÊHZ2[¿tñ=&•i‰¿:¿ˆ¹:½‹h8d0\8É§÷]ú¡¬uõÙr¢Å†Şç4uÊÒo9-Àçôä´ù‘ùpsi\d¿Î°S‚7¼í·Ò6üh}}_Êë¤ãÃ÷ÑW/ÍN3ëî|«1²;8¢ÿÑ}‰8_S'¥·™’IVù,³kZez¾ç³œR,Ôâü6±ek'ß²y÷=mæ[¿%Ñ€ş—|?Œ`{xÔn'ˆÓK—šÃñKsæÕÚÅ«¾Ö¿³/Z„ÌËäAg—Ì%ôjpÿ^ÍJºKÒÛQÚú¸3u_a
{f@ v€ßøßøßøßø7Åd"ó (  
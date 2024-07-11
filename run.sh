SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)

MODE="nemu | npc" 

USAGE="usage $0 {$MODE} [-b]"

IMAGE=$SHELL_FOLDER/output/fw/fw

if [ $# != 1 ] ; then
    if [ $# != 2 ] ; then
	    echo $USAGE
	    exit 1
    fi
fi
if [ $# = 2 ] ; then
    case "$2" in
    -b)
    	;;
    *)
    	echo $USAGE
    	exit 1	
    	;;
    esac
fi

case "$1" in
nemu)
	RUN_HOME=$NEMU_HOME
	;;
npc)
	RUN_HOME=$NPC_HOME
    ;;
--help)
	echo $USAGE
	exit 0
	;;
*)
	echo $USAGE
	exit 1	
	;;
esac

echo "make -C ${RUN_HOME} ISA="riscv64" run ARGS="--log=$SHELL_FOLDER/output/$1-log.txt --img=$SHELL_FOLDER/output/rootfs/rootfs.img $2" IMG=${IMAGE}.bin"
make -C ${RUN_HOME} ISA="riscv64" run ARGS="--log=$SHELL_FOLDER/output/$1-log.txt --img=$SHELL_FOLDER/output/rootfs/rootfs.img $2" IMG=${IMAGE}.bin

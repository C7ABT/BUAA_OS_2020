make
# gxemul -E testmips -C R3000 -M 64 gxemul/vmlinux
gxemul -E testmips -C R3000 -M 64 -d gxemul/fs.img gxemul/vmlinux


connect
targets -set -filter {name =~ "ARM*#0"}


fpga -file system.bit

# Run FSBL
dow zynq_fsbl.elf
con
after 2000   ;# wait ~2 seconds for U-Boot to start
stop

# Load U-Boot
dow u-boot-dtb.elf
con
after 25000   ;# wait ~2 seconds for U-Boot to start

# Load Linux image to DDR at 0x3000000
dow -data image.ub 0x3000000

puts "DownLoad complited please write \"bootm 0x3000000\" in your U-BOOT Terminal";
#-----------------Following from test.s---------------------#
.data
    
oxStr:        .asciiz "0x"
nlStr:      .asciiz "\n"
countStr1:    .asciiz " occurred "
countStr2:     .asciiz " times.\n"
badHexStr:    .asciiz "Value entered is not a proper hexadecimal number.\n"
val:        .word 0x00
input:        .space 20

.text

main:
    #Initialize the counts accesstable
    jal    createCountTable

nextNumber:    
    #Print '0x' as a prompt
    la    $a0 oxStr
    li    $v0 4
    syscall    

    #Read in a number
    la    $a0 input
    li    $a1 20
    li    $v0 8
    syscall
    
    #Call student code to parse it
    jal    readHex
    sw    $v0 val
    beq     $v1, $zero, goodHex

    #Bad hexdecimal value: print error message and go back for another number
    la      $a0, badHexStr
    li      $v0, 4
    syscall
    j       nextNumber
    
goodHex:    
    #Print the hex number using student code
    move    $a0 $v0
    jal printHex

    #Print the count info
    li    $v0 4
    la    $a0 countStr1
    syscall

    #Call student code to get the count for this one
    lw    $a0 val
    jal    countIntegerAccess    

    #Print out the remainder
    move    $a0 $v0
    li    $v0 1
    syscall
    
    li    $v0 4
    la     $a0 countStr2
    syscall
    
    #Loop back for another go!
    j    nextNumber
#----------------Until here from test.s---------------------#


.data
hexstr:                .space    8
count:                .word   0
accesstable:        .space  1200

.text

#input:
#none
#output:
#none
createCountTable:
    jr    $ra

# input: 
# $a0: 8 ASCII string
# output:
# $v0: hexadecimal number
# $v1: 0 for valid, 0 for non-valid number
readHex:
    move    $t0 $ra
    move    $t1 $a0
    # total 8 character, each 4 bits
    li        $t2 32
    # init the return number value to 0
    li      $t3 0
    
    loopstart:
        #load the first byte of $t1
        lb    $a1 0($t1)

        # in 0 - 9 (ASCII: 48 - 57)
        li    $a0 48
        li    $a2 57
        jal inRange
        beqz    $v0 charToHex
    
        # in A - F (ASCII: 65 - 70)
        li    $a0 65
        li     $a2 70
        jal    inRange
        beqz    $v0 charToHex
    
        # in a - f (ASCII: 97 - 102)
        li    $a0 97
        li     $a2 102
        jal    inRange
        beqz    $v0 charToHex

        # Invaild char, $v1 = 1
        li    $v1 1
        jr    $t0
        
    charToHex:
        # update store location and handled ASCII count
        addi    $t2 $t2 -4

        # ASCII -> hex
        sub    $a1 $a1 $a0
        beq $a0, 48, nonHex
        addi $a1 $a1 10

      nonHex:
        # store into $t3
        sllv    $a1 $a1 $t2
        or      $t3 $a1 $t3

        # ASCII over or not
        beqz    $t2 handleOver
        addi    $t1 $t1 1
        j       loopstart

    handleOver:
        # $v0 = $t3
        move    $v0 $t3
        # a valid hexdecimal number
        li  $v1 0
        jr  $t0

# input:
# a0: start of range
# a1: to judge value
# a2: end of range
# output:
# $v0, 0: in range, 1: out of range
inRange:
    sub    $v0 $a1 $a0
    # if $a0 <= $a1
    bltz    $v0 notInIt
    
    sub     $v0 $a2 $a1
    # if $a1 <= $a2
       bltz    $v0 notInIt

    # valid char, $v0 = 0
    li      $v0 0
    jr      $ra
    
  notInIt:
    #non valid char, $v0 = 1
    li    $v0 1
    jr    $ra
    

#input:
# $a0, hexdecimal number 
#output:
# none
printHex:
    #save $ra and $a0
    move $t0 $ra
    move $t1 $a0

    # get hexstr address
    la  $v1 hexstr

    # total 32 bits, each time get 4 bits
    li  $t2 28
    
  next:
    # set bit 1111
    li  $a1 15

    # get hex number by 4 bits
    sllv $a1 $a1 $t2
    and $a1 $a1 $t1
    srlv $a1 $a1 $t2

    li  $a0 10
    sub $v0 $a1 $a0

    # in 0-9
    li  $a0 48

    bltz $v0 decNum
    # in A-F
    li  $a0 87

  decNum:

    # store hex number into ASCII
    add $a1 $a1 $a0
    sb  $a1 0($v1)
    addi $v1 $v1 1

    # next hex number
    addi $t2 $t2 -4
    bltz $t2 hexOver
    jr next

  hexOver:
    # print out
    la $a0 hexstr
    li $v0 4
    syscall

    jr $t0

#input:
# $a0, to find integer value
#ouput:
# $v0, number of time the integer $a0 has been accessed
countIntegerAccess:
    # save $ra
    move    $t3 $ra

    # load accesstable address and count value
    la      $a1 accesstable
    lw      $a2 count

    # no element in accesstable
    beq     $a2 $0 notFound

    # first number in accesstable
    lw $t0 0($a1)

    # loop through the table
    findloop:
        # check the table element with $a0
        bne $t0 $a0 nextNum

        # find same element
        addi    $a1 4
        lw    $t0 0($a1)
        addi    $t0 1
        sw    $t0 0($a1)
        move    $v0 $t0

        # return the found access times
        jr    $t3

     nextNum:
        # next number in accesstable
        addi $a2 -1
        addi $a1 8

        # accesstable's end
        beq $a2 $0 notFound

        #load next number
        lw $t0 0($a1)

        j findloop

    notFound:
        # add number into accesstable

        # save number
        sw     $a0 0($a1)
        addi $a1 4

        # save access times
        li     $v0 1
        sb     $v0 0($a1)

        # add total number count
        lw     $t0 count
        addi $t0 1
        sw     $t0 count

        jr     $t3

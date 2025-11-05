if {$argc != 1} {
  puts "Input Error: format should be."
  puts "tclsh ssam.tcl filename.a"
  exit
}

if {![file exists [lindex $argv 0]]} {
  puts "Input Error: file [lindex $argv 0] does not exist."
}

if {[regexp {^(\w+)\.a$} [lindex $argv 0] match filename]} {
  puts "Loading file: [lindex $argv 0]"
} else {
  puts "File name invalid: file [lindex $argv 0]\" should follow \"filename.a\""
  exit
}

set destdir  "."

# set filename "exampleCode/example1.txt"
set fh [open "$destdir/$filename.a" r]

set wordsize              16
set verbose                0
set verify                 1
set address_label_assembly 1
set address_id_assembly    1
set address_id_binary      1
set raw_binary             1

set line_cnt               0

set label_names {}
array set detected_labels   {}
array set location_to_label {}
array set stored_assembly   {}
array set stored_binary     {}

proc code_line {line} { return [regexp {^\s*[a-zA-Z]} $line match] }

set memory_lower_bound "0x0000"
set memory_upper_bound "0xFFFF"


proc opcode_binary {op} {

  set retvalue ""

  switch -regexp $op {
    halt    { set retvalue "00000" }
    nop     { set retvalue "00001" }
    ret     { set retvalue "00010" }
    dump    { set retvalue "00100" }
    readr   { set retvalue "00101" }
    writr   { set retvalue "00110" }
    writa   { set retvalue "00111" }
    lodi    { set retvalue "01000" }
    loda    { set retvalue "01001" }
    lodrd   { set retvalue "01011" }
    lodr    { set retvalue "01010" }
    stoa    { set retvalue "01100" }
    stord   { set retvalue "01110" }
    stor    { set retvalue "01101" }
    neg     { set retvalue "10000" }
    addr    { set retvalue "10001" }
    addi    { set retvalue "10010" }
    subr    { set retvalue "10011" }
    subi    { set retvalue "10100" }
    mov     { set retvalue "10111" }
    jmpz    { set retvalue "1101" }
    jmpn    { set retvalue "1110" }
    jmp     { set retvalue "1100" }
    call    { set retvalue "1111" }
  }
  
  return $retvalue
}

proc dumpcode_binary {code} {

  set retvalue ""

  switch -regexp $code {
    all     { set retvalue "00" }
    reg     { set retvalue "01" }
    mem     { set retvalue "10" }
  }
  
  return $retvalue
}

proc register_binary {reg} {

  set retvalue ""

  switch -regexp $reg {
    r0      { set retvalue "000" }
    r1      { set retvalue "001" }
    r2      { set retvalue "010" }
    r3      { set retvalue "011" }
    R0      { set retvalue "000" }
    R1      { set retvalue "001" }
    R2      { set retvalue "010" }
    R3      { set retvalue "011" }
    ac      { set retvalue "100" }
    AC      { set retvalue "100" }
    sp      { set retvalue "101" }
    SP      { set retvalue "101" }
    bp      { set retvalue "110" }
    BP      { set retvalue "110" }
    pc      { set retvalue "111" }
    PC      { set retvalue "111" }
  }
  
  return $retvalue
}

proc hextobin_digit {hexdigit pad} {

  if {$pad} {
    switch -exact [string tolower $hexdigit] {
      0 { return "0000"}
      1 { return "0001"}
      2 { return "0010"}
      3 { return "0011"}
      4 { return "0100"}
      5 { return "0101"}
      6 { return "0110"}
      7 { return "0111"}
      8 { return "1000"}
      9 { return "1001"}
      a { return "1010"}
      b { return "1011"}
      c { return "1100"}
      d { return "1101"}
      e { return "1110"}
      f { return "1111"}
    }
  } else {
    switch -exact [string tolower $hexdigit] {
      0 { return "0"}
      1 { return "1"}
      2 { return "10"}
      3 { return "11"}
      4 { return "100"}
      5 { return "101"}
      6 { return "110"}
      7 { return "111"}
      8 { return "1000"}
      9 { return "1001"}
      a { return "1010"}
      b { return "1011"}
      c { return "1100"}
      d { return "1101"}
      e { return "1110"}
      f { return "1111"}
    }
  }

}

proc address_binary_8bits {addr} {
  
  set leading_zeros   1
  set first_iteration 1
  set value ""
  
  foreach digit [split $addr ""] {
    
    # if non-zero dectected unflag leading zeros
    if {$digit != "0"} {
      set leading_zeros 0
    }

    if {!$leading_zeros} {
      
      # do this if not leading zeros
      if {$first_iteration} {
        set value "$value[hextobin_digit $digit 0]"
        set first_iteration 0
      } else {
        set value "$value[hextobin_digit $digit 1]"
      }
    }
  }

  set width [string length $value]
  set padding ""
  
  for {set cnt 0} {$cnt < [expr 8 - $width]} {incr cnt} {
    set padding "0$padding"
  }
  
  return "$padding$value"
}

proc address_binary_11bits {addr} {
  
  set leading_zeros   1
  set first_iteration 1
  set value ""
  
  foreach digit [split $addr ""] {

    # if non-zero dectected unflag leading zeros
    if {$digit != "0"} {
      set leading_zeros 0
    }

    if {!$leading_zeros} {
      
      # do this if not leading zeros
      if {$first_iteration} {
        set value "$value[hextobin_digit $digit 0]"
        set first_iteration 0
      } else {
        set value "$value[hextobin_digit $digit 1]"
      }
    }
  }

  set width [string length $value]
  set padding ""
  
  for {set cnt 0} {$cnt < [expr 11 - $width]} {incr cnt} {
    set padding "0$padding"
  }
  
  return "$padding$value"
}

proc address_binary_12bits {addr} {
  
  set leading_zeros   1
  set first_iteration 1
  set value ""
  
  foreach digit [split $addr ""] {

    # if non-zero dectected unflag leading zeros
    if {$digit != "0"} {
      set leading_zeros 0
    }

    if {!$leading_zeros} {
      # do this if not leading zeros
      if {$first_iteration} {
        set value "$value[hextobin_digit $digit 0]"
        set first_iteration 0
      } else {
        set value "$value[hextobin_digit $digit 1]"
      }
    }
  }

  set width [string length $value]
  set padding ""
  
  for {set cnt 0} {$cnt < [expr 12 - $width]} {incr cnt} {
    set padding "0$padding"
  }
  
  return "$padding$value"
}

# Convert a decimal value to binary of a specific
# bit width.  This takes the 
proc dectobin {val digits} {

  set ret ""

  # Initialize the mask to have the bit at
  # position 1.
  set mask 0x1

  # Iterate for the number of positions.
  for {set x 0} {$x < $digits} {incr x} {

    # Check if the mask detects one or zero,
    # then set the bit appropriately.
    if {[expr $val & $mask]} {
      set ret "1$ret"
    } else {
      set ret "0$ret"
    }

    # Shift the mask by one bit-position to the left.
    set mask [expr $mask << 1]
  }

  return $ret
}


proc opcode_zeropad {op} {

  global wordsize
  
  set width [string length $op]
  
  set padding ""
  
  for {set cnt 0} {$cnt < [expr $wordsize - $width]} {incr cnt} {
    set padding "0$padding"
  }
  
  return "$op$padding"
}


# -- Takes a detected assembly instruction and adds to a list of
# -- lines identified by an address.
proc add_code {operation} {

  global line_cnt stored_assembly
  
  set address [format "%04x" $line_cnt]

  set stored_assembly($address) $operation
  
  # increment to next address
  incr line_cnt 2

}

# -- Takes a detected label and records it to track in the code.
proc add_label {label} {

  global detected_labels
  global location_to_label
  global line_cnt
  
  set curr_address [format "%04x" $line_cnt]

  set valid 1

  if {[info exists detected_labels($label)]} {
    puts "ERROR: label is a duplicate : \"$label\""
    set valid 0
  } else {

    # give the label a location
    set detected_labels($label) $curr_address

    # specify what label is at a location
    set location_to_label($curr_address) $label

  }
  
  return $valid
}

# -- Validate a hex address for an 11-bit range.
# --   0000 0000 0000 0000  0x0000  min value
# --   1111 1111 1111 1111  0xFFFF  max value
proc validate_hexAddress {addr} {

  global memory_lower_bound
  global memory_upper_bound

  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set trimmed_addr [string trim $addr]

  #-- validate hex address format
  #-- if not match tag string as invalid
  if {![regexp {^0x(\w+)$} $trimmed_addr match addr_val]} {
    puts "    ERROR address: hex address malformed"
    set valid 0
  }

  #-- validate hex value
  #-- if not match tag string as invalid
  if {$valid && ![string is xdigit $addr_val]} {
    puts "    ERROR address: hex address not a valid value"
    set valid 0
  }

  #-- validate address range
  #-- check only if addres valid to this point
  #-- check lower bound
  if {$valid && ![expr $memory_lower_bound < "0x$addr_val"]} {
    puts "    ERROR address: hex address less than lower_bound  0x0000"
    set valid 0
  }

  #-- check upper bound
  if {$valid && ![expr "0x$addr_val" <= $memory_upper_bound]} {
    puts "    ERROR address: hex address greater than upper_bound 0x07FF"
    puts "                   value 0\x$addr_val"
    set valid 0
  }

  #-- exit proceedure
  return $valid
}


# -- Validate a hex address for an 11-bit range.
# --   000 0000 0000  0x0000  min value
# --   111 1111 1111  0x07FF  max value
proc validate_hexAddress_11bit {addr} {

  set lower_bound "0x0000"
  set upper_bound "0x07FF"

  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set trimmed_addr [string trim $addr]

  #-- validate hex address format
  #-- if not match tag string as invalid
  if {![regexp {^0x(\w+)$} $trimmed_addr match addr_val]} {
    puts "    ERROR address: 11-bit hex address malformed"
    set valid 0
  }

  #-- validate hex value
  #-- if not match tag string as invalid
  if {$valid && ![string is xdigit $addr_val]} {
    puts "    ERROR address: hex address not a valid value"
    set valid 0
  }

  #-- validate address range
  #-- check only if addres valid to this point
  #-- check lower bound
  if {$valid && ![expr $lower_bound < "0x$addr_val"]} {
    puts "    ERROR address: hex address less than lower_bound  0x0000"
    set valid 0
  }

  #-- check upper bound
  if {$valid && ![expr "0x$addr_val" <= $upper_bound]} {
    puts "    ERROR address: 11-bit hex address greater than upper_bound 0x07FF"
    puts "                   value 0\x$addr_val"
    set valid 0
  }

  #-- exit proceedure
  return $valid
}


# -- Validate a hex address for an 11-bit range.
# --   0000 0000 0000  0x0000  min value
# --   1111 1111 1111  0x07FF  max value
proc validate_hexAddress_12bit {addr} {

  set lower_bound "0x0000"
  set upper_bound "0x0FFF"

  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set trimmed_addr [string trim $addr]

  #-- validate hex address format
  #-- if not match tag string as invalid
  if {![regexp {^0x(\w+)$} $trimmed_addr match addr_val]} {
    puts "    ERROR address: 11-bit hex address malformed"
    set valid 0
  }

  #-- validate hex value
  #-- if not match tag string as invalid
  if {$valid && ![string is xdigit $addr_val]} {
    puts "    ERROR address: hex address not a valid value"
    set valid 0
  }

  #-- validate address range
  #-- check only if addres valid to this point
  #-- check lower bound
  if {$valid && ![expr $lower_bound < "0x$addr_val"]} {
    puts "    ERROR address: hex address less than lower_bound  0x0000"
    set valid 0
  }

  #-- check upper bound
  if {$valid && ![expr "0x$addr_val" <= $upper_bound]} {
    puts "    ERROR address: 12-bit hex address greater than upper_bound 0x0FFF"
    puts "                   value 0\x$addr_val"
    set valid 0
  }

  #-- exit proceedure
  return $valid
}


# -- Validate a hex address for an 8-bit range.
# --   0000 0000  0x00  min value
# --   1111 1111  0xFF  max value
proc validate_hexAddress_8bit {addr} {
  
  set lower_bound "0x00"
  set upper_bound "0xFF"

  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set trimmed_addr [string trim $addr]

  #-- validate hex address format
  #-- if not match tag string as invalid
  # puts "trimmed_addr  $trimmed_addr"
  
  if {$valid && ![regexp {^0x(\w+)$} $trimmed_addr match addr_val]} {
    puts "    ERROR address: 8-bit hex address malformed:  $addr"
    set valid 0
  }

  #-- validate hex value
  #-- if not match tag string as invalid
  if {$valid && ![string is xdigit $addr_val]} {
    puts "    ERROR address: hex address not a valid value"
    set valid 0
  }

  #-- validate address range
  #-- check only if addres valid to this point
  #-- check lower bound
  if {$valid && ![expr $lower_bound < "0x$addr_val"]} {
    puts "    ERROR address: hex address less than lower_bound  0x00"
    set valid 0
  }

  #-- check upper bound
  if {$valid && ![expr "0x$addr_val" <= $upper_bound]} {
    puts "    ERROR address: 8-bit hex address greater than upper_bound $upper_bound"
    puts "                   value 0\x$addr_val"
    set valid 0
  }

  #-- exit proceedure
  return $valid
}


# -- Validate a immediate value in the 8-bit range.
# --   000 0000 0000  0x0000  min value
# --   111 1111 1111  0x07FF  max value
proc validate_immediate_8bit {addr} {

  set lower_bound -128
  set upper_bound 127

  set value 0
  
  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set trimmed_addr [string trim $addr]

  if {[regexp {0x} $trimmed_addr]} {

    #-- validate hex address format
    #-- if not match tag string as invalid
    if {![regexp {^0x(\w+)$} $trimmed_addr match addr_val]} {
      puts "    ERROR address: 8-bit immediate value malformed"
      set valid 0
    }

    #-- validate hex value
    #-- if not match tag string as invalid
    if {$valid && ![string is xdigit $addr_val]} {
      puts "    ERROR address: 8-bit immediate value not a valid value"
      set valid 0
    }
  
    set value [format %d $trimmed_addr]
    
  } else {

    #-- validate dec address format
    #-- if not match tag string as invalid
    if {![string is integer $trimmed_addr]} {
      puts "    ERROR address: decimal address not a valid value"
      set valid 0
    }
    
    set value $trimmed_addr
  }

  #-- validate address range
  #-- check only if addres valid to this point
  #-- check lower bound
  if {$valid && ![expr $lower_bound < $value]} {
    puts "    ERROR address: immediate value less than lower_bound  0x00"
    set valid 0
  }

  #-- check upper bound
  if {$valid && ![expr $value <= $upper_bound]} {
    puts "    ERROR address: immediate value greater than upper_bound $upper_bound"
    set valid 0
  }

  #-- exit proceedure
  return $valid
}


# -- Validate a index value in the 5-bit range.
# --   0 0000  0x00  min value
# --   1 1111  0x1F  max value
proc validate_index_5bit {addr} {

  set lower_bound -16
  set upper_bound 15

  set value 0
  
  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set trimmed_addr [string trim $addr]

  if {[regexp {0x} $trimmed_addr]} {

    #-- validate hex address format
    #-- if not match tag string as invalid
    if {![regexp {^0x(\w+)$} $trimmed_addr match addr_val]} {
      puts "    ERROR address: 5-bit hex address malformed"
      set valid 0
    }

    #-- validate hex value
    #-- if not match tag string as invalid
    if {$valid && ![string is xdigit $addr_val]} {
      puts "    ERROR address: 5-bit hex address not a valid value"
      set valid 0
    }
  
    set value [format %d $trimmed_addr]
    
  } else {

    #-- validate dec address format
    #-- if not match tag string as invalid
    if {![string is integer $trimmed_addr]} {
      puts "    ERROR address: decimal address not a valid value"
      set valid 0
    }
    
    set value $trimmed_addr
  }

  #-- validate address range
  #-- check only if addres valid to this point
  #-- check lower bound
  if {$valid && ![expr $lower_bound < $value]} {
    puts "    ERROR address: immediate value less than lower_bound  0x00"
    set valid 0
  }

  #-- check upper bound
  if {$valid && ![expr $value <= $upper_bound]} {
    puts "    ERROR address: immediate value greater than upper_bound $upper_bound"
    set valid 0
  }

  #-- exit proceedure
  return $valid
}


# -- Validate a register name that can be either
# -- upper or lower case.
proc validate_register {reg_name} {

  #-- Assume valid return
  set valid 0

  #-- remove any unneeded space
  set name [string trim $reg_name]

  switch -regexp $name {
    r0      { set valid 1 }
    r1      { set valid 1 }
    r2      { set valid 1 }
    r3      { set valid 1 }
    R0      { set valid 1 }
    R1      { set valid 1 }
    R2      { set valid 1 }
    R3      { set valid 1 }
    ac      { set valid 1 }
    AC      { set valid 1 }
    sp      { set valid 1 }
    SP      { set valid 1 }
    bp      { set valid 1 }
    BP      { set valid 1 }
    pc      { set valid 1 }
    PC      { set valid 1 }
    default { set valid 0 }
  }
  
  #-- exit proceedure
  return $valid
}


# -- Validate a dump code name.
proc validate_dump_code {code} {

  #-- Assume valid return
  set valid 1

  #-- remove any unneeded space
  set name [string trim $code]

  switch -regexp $name {
    all     { set valid 1 }
    mem     { set valid 1 }
    reg     { set valid 1 }
    default { set valid 0 }
  }

  #-- exit proceedure
  return $valid
}



# -- Validate the halt operation, which will only fail if
# -- there is more on the line than the word halt. (This is
# -- before a comment.)
proc halt_op   {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "halt  : $line" }

  # verify
  if {![regexp {^\s*halt\s*$} $line]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "halt"


  # return $valid
}

# -- Validate the nop operation, which will only fail if
# -- there is more on the line than the word nop. (This is
# -- before a comment.)
proc nop_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "nop   : $line" }

  # verify
  if {![regexp {^\s*nop\s*$} $line]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }
  
  if {!$valid } { return $valid }

  # generate code
  add_code "nop"
  
  # return $valid
}

# -- Validate the ret operation, which will only fail if
# -- there is more on the line than the word ret. (This is
# -- before a comment.)
proc ret_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "ret     : $line" }

  # verify
  if {![regexp {^\s*ret\s*$} $line]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "ret"
  
  # return $valid
}

proc dump_op {line} {
  
  global verbose verify \# control variables
  
  # verbose
  if {$verbose} { puts "dump    : $line" }

  #-- Assume valid return
  set valid 1

  # verify
  if {![regexp {^\s*dump\s+(\w+)\s*$} $line match code]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate dump code
  if {![validate_dump_code $code]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "dump $code"
  
  # return $valid
}

proc readr_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "readr     : $line" }

  # verify
  if {![regexp {^\s*readr\s+(\w+)\s*$} $line match rega]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "readr $rega"

  # return $valid
}

# -- before a comment.)
proc writr_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "writr     : $line" }

  # verify
  if {![regexp {^\s*writr\s+(\w+)\s*$} $line match rega]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {![validate_register $rega]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "writr $rega"

  # return $valid
}

proc writa_op {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "writa     : $line" }

  # verify
  if {![regexp {^\s*writa\s+(\w+)\s*$} $line match addr]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate address
  if {![validate_hexAddress_11bit $addr]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "writa $addr"

  return $valid
}

proc lodi_op   {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "lodi   : $line" }

  # verify
  if {![regexp {^\s*lodi\s+(\w+)\s*\,\s+(\w+)\s*$} $line match rega imm]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate immediate
  if {$valid && ![validate_immediate_8bit $imm]} {
    set valid 0
  }
  
  if {!$valid } { return $valid }

  # generate code
  add_code "lodi $rega, $imm"
  
  # return $valid
}

proc loda_op   {line} {
  
  global verbose verify # control variables
  global label_names

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "loda   : $line" }

  # verify
  if {![regexp {^\s*loda\s+(\w+)\s*\,\s+\((\w+)\)\s*$} $line match rega addr]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate address value
  if {[lsearch $label_names $addr] == -1} {
    if {$valid && ![validate_hexAddress_8bit $addr]} {
      set valid 0
    }
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "loda $rega, ($addr)"

  # return $valid
}

proc lodr_op   {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "lodr   : $line" }

  # verify
  if {![regexp {^\s*lodr\s+(\w+)\s*\,\s+\(\s*(\w+)\s*\)\s*$} $line match rega regb]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "lodr $rega, ($regb)"

  # return $valid
}

proc lodrd_op  {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "lodrd  : $line" }

  # verify
  if {![regexp {^\s*lodrd\s+(\w+)\s*\,\s+\(\s*(\w+)\s+\+\s+(-?\w+)\s*\)\s*$} $line match rega regb ind]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  # validate index
  if {$valid && ![validate_index_5bit $ind]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "lodrd $rega, ($regb + $ind)"

  # return $valid
}

proc stoa_op   {line} {
  
  global verbose verify \# control variables
  global label_names

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "stoa   : $line" }

  # verify
  if {![regexp {^\s*stoa\s+\(\s*(\w+)\s*\)\s*\,\s+(\w+)\s*$} $line match addr rega]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate address value
  if {[lsearch $label_names $addr] == -1} {
    if {$valid && ![validate_hexAddress_8bit $addr]} {
      set valid 0
    }
  }
  
  if {!$valid } { return $valid }

  # generate code
  add_code "stoa ($addr), $rega"

  # return $valid
}

proc stor_op   {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "stor   : $line" }

  # verify
  if {![regexp {^\s*stor\s+\(\s*(\w+)\s*\)\s*\,\s+(\w+)\s*$} $line match regb rega]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "stor ($regb), $rega"

  # return $valid
}

proc stord_op  {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "stord  : $line" }

  # verify
  if {![regexp {^\s*stord\s+\(\s*(\w+)\s+\+\s+(-?\w+)\s*\)\s*\,\s+(\w+)\s*$} $line match regb ind rega]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  # validate index
  if {$valid && ![validate_index_5bit $ind]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "stord ($regb + $ind), $rega"

  # return $valid
}

proc addr_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "addr  : $line" }

  # verify
  if {![regexp {^\s*addr\s+(\w+)\s*\,\s+(\w+)\s*$} $line match rega regb]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "addr $rega, $regb"

  # return $valid
}

proc addi_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "addi  : $line" }

  # verify
  if {![regexp {^\s*addi\s+(\w+)\s*\,\s+(\w+)\s*$} $line match rega imm]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate immediate
  if {$valid && ![validate_immediate_8bit $imm]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "addi $rega, $imm"

  # return $valid
}

proc subr_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "subr  : $line" }

  # verify
  if {![regexp {^\s*subr\s+(\w+)\s*\,\s+(\w+)\s*$} $line match rega regb]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "subr $rega, $regb"

  # return $valid
}

proc subi_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "subi  : $line" }

  # verify
  if {![regexp {^\s*subi\s+(\w+)\s*\,\s+(\w+)\s*$} $line match rega imm]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate immediate
  if {$valid && ![validate_immediate_8bit $imm]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "subi $rega, $imm"

  # return $valid
}

proc neg_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "neg  : $line" }

  # verify
  if {![regexp {^\s*neg\s+(\w+)\s*$} $line match rega ]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "neg $rega"

  # return $valid
}

proc mov_op    {line} {
  
  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "mov  : $line" }

  # verify
  if {![regexp {^\s*mov\s+(\w+)\s*\,\s+(\w+)\s*$} $line match rega regb]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate register a
  if {$valid && ![validate_register $rega]} {
    set valid 0
  }

  # validate register b
  if {$valid && ![validate_register $regb]} {
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "mov $rega, $regb"
  
  # return $valid
}

proc jmp_op    {line} {
  
  global verbose verify # control variables
  global label_names

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "jmp  : $line" }

  # verify
  if {![regexp {^\s*jmp\s+(\w+)\s*$} $line match addr]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }
  
  # validate address value
  if {[lsearch $label_names $addr] == -1} {
    if {$valid && ![validate_hexAddress_12bit $addr]} {
      set valid 0
    }
  }
  
  if {!$valid } { return $valid }

  # generate code
  add_code "jmp $addr"

  # return $valid
}

proc jmpz_op   {line} {
  
  global verbose verify # control variables
  global label_names

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "jmpz : $line" }

  # verify
  if {![regexp {^\s*jmpz\s+(\w+)\s*$} $line match addr]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate address value
  if {[lsearch $label_names $addr] == -1} {
    if {$valid && ![validate_hexAddress_12bit $addr]} {
      set valid 0
    }
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "jmpz $addr"

  # return $valid
}

proc jmpn_op   {line} {
  
  global verbose verify # control variables
  global label_names

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "jmpn : $line" }

  # verify
  if {![regexp {^\s*jmpn\s+(\w+)\s*$} $line match addr]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate address value
  if {[lsearch $label_names $addr] == -1} {
    if {$valid && ![validate_hexAddress_12bit $addr]} {
      set valid 0
    }
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "jmpn $addr"

  # return $valid
}

proc call_op   {line} {
  
  global verbose verify # control variables
  global label_names

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "call    : $line" }

  # verify
  if {![regexp {^\s*call\s+(\w+)\s*$} $line match addr]} {
    puts "ERROR: operation malformed : \"$line\""
    set valid 0
  }

  # validate address value
  if {[lsearch $label_names $addr] == -1} {
    if {$valid && ![validate_hexAddress_12bit $addr]} {
      set valid 0
    }
  }

  if {!$valid } { return $valid }

  # generate code
  add_code "call $addr"

  # return $valid
}

# -- Process labels, tracking their locations and
# -- provide translations.
# --   Ensure there are no duplicates.
# --   Store label to address and address to label.
proc label_cmd {line} {

  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "label   : $line" }

  # verify
  #[regexp {^\s*([:alpha:][\w\_]*)\:\s*$} $line match label_name]
  if {![regexp {^\s*(\w[\w\_]*)\:\s*$} $line match label_name]} {
    puts "ERROR: operation malformed label : \"$line\""
    set valid 0
  }

  if {!$valid } { return $valid }

  # generate code
  add_label $label_name

  # return $valid
}

# -- Validates a directive and resolves action on the resulting assembly.
proc dot_cmd   {line} {

  global verbose verify \# control variables
  global line_cnt
  global stored_binary

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "dotcmd  : $line" }
  
  # verify
  if {![regexp {^\s*\.(\w+).*$} $line match directive]} {
    puts "ERROR: directive malformed : \"$line\""
    set valid 0
  }

  switch -regexp $directive {
  
    pos {
      
      # verify that directive is properly formed
      if {![regexp {^\s*\.(\w+)\s+(\w+)\s*$} $line match directive pos]} {
        puts "ERROR: pos directive malformed : \"$line\""
        set valid 0
      }
      
      # verify that directive address is properly formed
      if {$valid && ![validate_hexAddress_12bit $pos]} {
        puts "ERROR: pos directive malformed : \"$line\""
        set valid 0
      }
      
      # jump to new identified line
      set line_cnt $pos
    }

    short {
      # load a short into current line number in the binary storage
      # this is different from how commands are normally loaded, since
      # this 

      # verify that directive is properly formed
      if {![regexp {^\s*\.(\w+)\s+(\w+)\s*$} $line match directive value]} {
        puts "ERROR: short directive malformed : \"$line\""
        set valid 0
      }
      
      # determine current address in the correct format
      set current_address [format "%04x" $line_cnt]

      # store the value to the stored_binary, which is down-stream 
      # from the other item storage (like assembly ops).
      set stored_binary($current_address) [dectobin $value 16]
      
      # increment to next address
      incr line_cnt 2
    }
    
    default {
      puts "ERROR: unknown directive : \"$directive\""
      set valid 0
    }
  }

  return $valid
}

proc comment   {line} {

  global verbose verify # control variables

  #-- Assume valid return
  set valid 1
  
  # verbose
  if {$verbose} { puts "comment : $line" }
  
  # verify
  if {!$valid } { return $valid }

  # generate code

  # return $valid
}

while {![eof $fh]} {
  
  set line [gets $fh]
  
  if {[regexp ":" $line]} {
    # prev: [regexp {^\s*([:alpha:][\w\_]*)\:\s*$} $line match label_name]
    if {[regexp {^\s*(\w[\w\_]*)\:\s*$} $line match label_name]} {
      lappend label_names $label_name
    } else {
      puts "can not be parse for regular expression >>$line<<"
    }
    
  }
}

# reset the input file to the start
seek $fh 0

while {![eof $fh]} {
  
  set line [gets $fh]

  # removing comment
  if {[regexp ";" $line]} {
    set splitline [split $line ";"]
    set line [lindex $splitline 0]
  }
  
  switch -regexp $line {
    halt    { halt_op   $line }
    nop     { nop_op    $line }
    ret     { ret_op    $line }
    dump    { dump_op   $line }
    readr   { readr_op  $line }
    writr   { writr_op  $line }
    writa   { writa_op  $line }
    lodi    { lodi_op   $line }
    loda    { loda_op   $line }
    lodrd   { lodrd_op  $line }
    lodr    { lodr_op   $line }
    stoa    { stoa_op   $line }
    stord   { stord_op  $line }
    stor    { stor_op   $line }
    addr    { addr_op   $line }
    addi    { addi_op   $line }
    subr    { subr_op   $line }
    subi    { subi_op   $line }
    neg     { neg_op    $line }
    mov     { mov_op    $line }
    jmpz    { jmpz_op   $line }
    jmpn    { jmpn_op   $line }
    jmp     { jmp_op    $line }
    call    { call_op   $line }
    ":"     { label_cmd $line }
    {\.\w+} { 
      if {![dot_cmd   $line]} {
        puts "ERROR: directive invalid: $line"
        exit
      }
    }
    {\w+}   { puts "unknown operation: \"$line\"" }
    {\s+}   {
      if {$verbose} { puts "empty   : $line" }
    }
    default { 
      
      if {$verbose} {
        if {[string length $line] == 0} { 
          puts "empty   : $line" 
        } else {
          puts "unknown : $line"
        }
      }
    }
  }
}

close $fh

#-- Display assembly code with addresses, but labels not resolved.
if {$address_label_assembly} {
  
  set lafh [open "$destdir/$filename.la" w]
  
  puts $lafh ".file $filename.la"
  puts $lafh ".format SSAM-label-assembly"
  
  foreach name [lsort [array names stored_assembly]] {

    set addressVal "0x$name" 

    if {[info exists location_to_label($name)]} {
      puts $lafh "\n        $location_to_label($name):"
    }

    puts $lafh "$addressVal    $stored_assembly($name)"
  }
}
  
# #-- Generate assembly code with addresses and labels resolved.
# if {$address_id_assembly} {
#
#   #-- If needed display display file formats.
#   puts ".file $filename"
#   puts ".format SSAM-address-assembly 1"
# }

foreach name [lsort [array names stored_assembly]] {

  set addressVal "0x$name" 

  #-- If needed display labels resolved.
  if {$address_id_assembly} {
    if {[info exists location_to_label($name)]} {
      # puts "\n        $location_to_label($name):"
    }
  }


  set split_assem [split $stored_assembly($name)]

  switch -regexp $stored_assembly($name) {
    stoa  -
    loda  {

      # parse operation
      if {![regexp {^(.+\()(.+)(\).*)$} $stored_assembly($name) match front middle back ]} {
        puts "ERROR: operation could not be parsed : \"$stored_assembly($name)\""
        exit
      }
      
      # determine label and replace with stored address
      set labelIndex [lsearch $label_names $middle]
      if {$labelIndex > -1} {
        set label_value "0x$detected_labels($middle)"
        set stored_assembly($name) "$front$label_value$back"
      }
    }
    jmp   -
    jmpz  -
    jmpn  -
    call  {

      # parse operation
      if {![regexp {^(\w+\s)(.+)$} $stored_assembly($name) match front back ]} {
        puts "ERROR: operation could not be parsed : \"$stored_assembly($name)\""
        exit
      }

      # determine label and replace with stored address
      set labelIndex [lsearch $label_names $back]
      if {$labelIndex > -1} {
        set label_value "0\x$detected_labels($back)"
        set stored_assembly($name) "$front$label_value"
      }
    }
  }
}

#-- If needed display the assembly code with addresses and labels resolved.
if {$address_id_assembly} {
  # puts "$addressVal    $stored_assembly($name)"
}


# ---------------------------------------------------------------------------
#-- Display assembly code with addresses, encoded assembly, and assembly.

if {$address_id_binary} {

  set lbafh [open "$destdir/$filename.lba" w]

  puts $lbafh ".file $filename.lba"
  puts $lbafh ".format SSAM-labeled-binary-assembly 2"
}
  

  foreach name [lsort [array names stored_assembly]] {

    set addressVal "0x$name" 

    if {$address_id_binary} {
      if {[info exists location_to_label($name)]} {
        puts $lbafh "\n        $location_to_label($name):"
      }
    }

    # set split_assem [split $stored_assembly($name)]
    # puts "$name  :: $stored_assembly($name)"
    switch -regexp $stored_assembly($name) {
      halt    { set stored_binary($name) [opcode_zeropad [opcode_binary "halt"]] }
      nop     { set stored_binary($name) [opcode_zeropad [opcode_binary "nop" ]] }
      ret     { set stored_binary($name) [opcode_zeropad [opcode_binary "ret" ]] }
      dump    {
        
        # op code encoding
        set temp [opcode_binary "dump"  ]

        # parse formated instruction
        if {[regexp {^dump\s(\w+)$} $stored_assembly($name) match code]} {

          # construct machine instruction
          set temp "$temp[dumpcode_binary $code]"
          set stored_binary($name) [opcode_zeropad $temp] 
        }
      }
      readr   { 

        # op code encoding
        set temp [opcode_binary "readr"  ]

        # parse formated instruction
        if {[regexp {^readr\s(\w+)$} $stored_assembly($name) match reg]} {

          # construct machine instruction
          set temp "$temp[register_binary $reg]"
          set stored_binary($name) [opcode_zeropad $temp] 
        }
      }
      writr   { 

        # op code encoding
        set temp [opcode_binary "writr"  ]

        # parse formated instruction
        if {[regexp {^writr\s(\w+)$} $stored_assembly($name) match reg]} {

          # construct machine instruction
          set temp "$temp[register_binary $reg]"
          set stored_binary($name) [opcode_zeropad $temp] 
        }
      }
      writa   { 

        # op code encoding
        set temp [opcode_binary "writa"  ]

        # parse formated instruction
        if {[regexp {^writa\s0x(\w+)$} $stored_assembly($name) match addr]} {

          # construct machine instruction
          set temp "$temp[address_binary_11bits $addr]"
          set stored_binary($name) $temp
        }
      }
      lodi    { 

        # op code encoding
        set temp [ opcode_binary "lodi"  ]

        # parse formated instruction
        if {[regexp {^lodi\s(\w+)\,\s(\w+)$} $stored_assembly($name) match rega imm]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[dectobin $imm 8]"
          set stored_binary($name) $temp
        }
        
       }
      loda    { 

        # op code encoding
        set temp [ opcode_binary "loda"  ]

        # parse formated instruction
        if {[regexp {^loda\s(\w+)\,\s\(0x(\w+)\)$} $stored_assembly($name) match rega addr]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[address_binary_8bits $addr]"
          set stored_binary($name) $temp
        }
        
       }
      lodrd   { 
        
        # op code encoding
        set temp [ opcode_binary "lodrd"  ]

        # parse formated instruction
        if {[regexp {^lodrd\s(\w+)\,\s\((\w+)\s\+\s(-?\w+)\)$} $stored_assembly($name) match rega regb ind]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set temp "$temp[dectobin $ind 5]"
          set stored_binary($name) $temp
        }
      }
      lodr    { 

        # op code encoding
        set temp [ opcode_binary "lodr"  ]

        # parse formated instruction
        if {[regexp {^lodr\s(\w+)\,\s\((\w+)\)$} $stored_assembly($name) match rega regb]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set stored_binary($name) [opcode_zeropad $temp]
        }
      }
      stoa    {

        # op code encoding
        set temp [ opcode_binary "stoa"  ]

        # parse formated instruction
        if {[regexp {^stoa\s\(0x(\w+)\),\s(\w+)$} $stored_assembly($name) match addr rega]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[address_binary_8bits $addr]"
          set stored_binary($name) $temp
        }
      }
      stord   { 
        
        # op code encoding
        set temp [ opcode_binary "stord"  ]

        # parse formated instruction
        if {[regexp {^stord\s\((\w+)\s\+\s(-?\w+)\),\s(\w+)$} $stored_assembly($name) match regb ind rega ]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set temp "$temp[dectobin $ind 5]"
          set stored_binary($name) $temp
        }
      }
      stor    {
        
        # op code encoding
        set temp [ opcode_binary "stor"  ]

        # parse formated instruction
        if {[regexp {^stor\s\((\w+)\)\,\s(\w+)$} $stored_assembly($name) match regb rega]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set stored_binary($name) [opcode_zeropad $temp]
        }

      }
      addr    {
        
        # op code encoding
        set temp [ opcode_binary "addr"  ]

        # parse formated instruction
        if {[regexp {^addr\s(\w+)\,\s(\w+)$} $stored_assembly($name) match rega regb]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set stored_binary($name) [opcode_zeropad $temp]
        }
      }
      addi    { 

        # op code encoding
        set temp [ opcode_binary "addi"  ]

        # parse formated instruction
        if {[regexp {^addi\s(\w+)\,\s(\w+)$} $stored_assembly($name) match rega imm]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[dectobin $imm 8]"
          set stored_binary($name) $temp
        }
      }
      subr    { 
        
        # op code encoding
        set temp [ opcode_binary "subr"  ]

        # parse formated instruction
        if {[regexp {^subr\s(\w+)\,\s(\w+)$} $stored_assembly($name) match rega regb]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set stored_binary($name) [opcode_zeropad $temp]
        }
      }
      subi    { 

        # op code encoding
        set temp [ opcode_binary "subi"  ]

        # parse formated instruction
        if {[regexp {^subi\s(\w+)\,\s(\w+)$} $stored_assembly($name) match rega imm]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[dectobin $imm 8]"
          set stored_binary($name) $temp
        }
      }
      neg     {
        # op code encoding
        set temp [ opcode_binary "neg"  ]

        # parse formated instruction
        if {[regexp {^neg\s(\w+)$} $stored_assembly($name) match rega]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set stored_binary($name) [opcode_zeropad $temp]
        }
      }
      mov     { 
        # op code encoding
        set temp [ opcode_binary "mov"  ]

        # parse formated instruction
        if {[regexp {^mov\s(\w+)\,\s(\w+)$} $stored_assembly($name) match rega regb]} {

          # construct machine instruction
          set temp "$temp[register_binary $rega]"
          set temp "$temp[register_binary $regb]"
          set stored_binary($name) [opcode_zeropad $temp]
        }
      }
      jmpz    {

        # op code encoding
        set temp [opcode_binary "jmpz"  ]

        # parse formated instruction
        if {[regexp {^jmpz\s0x(\w+)$} $stored_assembly($name) match addr]} {
          # construct machine instruction
          set temp "$temp[address_binary_12bits $addr]"
          set stored_binary($name) $temp
        }
      }
      jmpn    {
        
        # op code encoding
        set temp [opcode_binary "jmpn"  ]

        # parse formated instruction
        if {[regexp {^jmpn\s0x(\w+)$} $stored_assembly($name) match addr]} {

          # construct machine instruction
          set temp "$temp[address_binary_12bits $addr]"
          set stored_binary($name) $temp
        }
      }
      jmp     {
        
        # op code encoding
        set temp [opcode_binary "jmp"  ]

        # parse formated instruction
        if {[regexp {^jmp\s0x(\w+)$} $stored_assembly($name) match addr]} {

          # construct machine instruction
          set temp "$temp[address_binary_12bits $addr]"
          set stored_binary($name) $temp
        }
      }
      call    {
        
        # op code encoding
        set temp [opcode_binary "call"  ]

        # parse formated instruction
        if {[regexp {^call\s0x(\w+)$} $stored_assembly($name) match addr]} {

          # construct machine instruction
          set temp "$temp[address_binary_12bits $addr]"
          set stored_binary($name) $temp
        }
      }
    }
    
    if {$address_id_binary} {
      puts $lbafh "$addressVal  >[string length $stored_binary($name)]>>$stored_binary($name)<<< [format "0x%04X" "0b$stored_binary($name)"] $stored_assembly($name)"
    }
  }
  
  
proc bintohex_digit {digit} {

  switch -exact $digit {
    0000 { return "0" }
    0001 { return "1" }
    0010 { return "2" }
    0011 { return "3" }
    0100 { return "4" }
    0101 { return "5" }
    0110 { return "6" }
    0111 { return "7" }
    1000 { return "8" }
    1001 { return "9" }
    1010 { return "a" }
    1011 { return "b" }
    1100 { return "c" }
    1101 { return "d" }
    1110 { return "e" }
    1111 { return "f" }
  }
}

proc bintohex_2bytes {code} {

  set bit_list [split $code ""]

  if {[llength $bit_list] != 16} {
    puts "ERROR: list not 16 bits"
    exit 1
  }

  set nibble3 [bintohex_digit [join [lrange $bit_list  0  3] "" ]]
  set nibble2 [bintohex_digit [join [lrange $bit_list  4  7] "" ]]
  set nibble1 [bintohex_digit [join [lrange $bit_list  8 11] "" ]]
  set nibble0 [bintohex_digit [join [lrange $bit_list 12 15] "" ]]

  return "0x$nibble3$nibble2$nibble1$nibble0"
  # return [list "0x$nibble3$nibble2" "0x$nibble1$nibble0"]
}


#-- Display full executable image.
if {$raw_binary} {

  set fhob [open "$destdir/$filename.bin" w]
  # set fhob [open binary.bin w]

  chan configure $fhob -translation binary

  # print the majic number as two bytes
  puts -nonewline $fhob [binary format S1  "0xbaab"]

  set prev 0
  foreach instr_pos [lsort [array names stored_binary]] {

    set prev_pos [format "%d" "0x$prev"]
    set next_pos [format "%d" "0x$instr_pos"]
    set pos_diff [expr $next_pos - $prev_pos]

    if {$pos_diff > 2} {
      # puts "-------- $pos_diff ----------"
      for {set x 0} {$x < $pos_diff - 2} {incr x} {
        puts -nonewline $fhob [binary format c1  "0x00"]
      }
    }

    puts -nonewline $fhob [binary format S1 [bintohex_2bytes $stored_binary($instr_pos)]  ]
    set prev $instr_pos
  }
  
  close $fhob
}
  


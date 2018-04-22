# bit8


## I-type
<table>
   <tr>
      <td>1</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
</tr>
<tr>
      <td>type</td>
      <td>op</td>
      <td colspan=2>reg</td>
      <td colspan=4>value</td>
</tr>
</table>

**I-Type** instructions are for setting registers to values.  
there are two operations.

### Operations
1. upper (set the 4 highest significant bits of *reg* to value)
2. lower (set the 4 lowest significant bits of *reg* to value)

## E-type

<table>
   <tr>
      <td>0</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
</tr>
<tr>
      <td>type</td>
      <td colspan=3>op</td>
      <td colspan=2>reg A</td>
      <td colspan=2>reg B</td>
</tr>
</table>

**E-type** instructions are for operations that require two registers.  
There are 7 operations in total, since the 000-operator is used to switch to B-type instructions.  
### Operations
0. B-type
1. addition (B = B + A)
2. subtraction (B = B - A)
3. multiplication (B = B * A)
4. Branch if Equals (jump to address in register 3 if B == A)
5. Set Less Than (B = B < A)
6. AND (B = B & A)
7. OR (B = B | A)

## B-type

<table>
   <tr>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
      <td>X</td>
</tr>
<tr>
      <td>type</td>
      <td colspan=3>sub-type</td>
      <td colspan=2>op</td>
      <td colspan=2>reg</td>
</tr>
</table>

**B-type** instructions are for operations that require one register.  
There are 4 operations in total
### Operations
0. Out (write the data out, implentation-specific), or no-op
1. Push (push the value in *reg* to the memory-stack)
2. Pop (pop the value from the memory-stack into *reg*)
3. Jump (set the program counter to the value in *reg*)

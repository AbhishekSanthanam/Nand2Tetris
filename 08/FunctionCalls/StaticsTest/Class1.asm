(Class1.set)
@0
D=A
(lstartClass1.set0)
@loopendClass1.set0
D;JEQ
@SP
A=M
M=0
@SP
M=M+1
D=D-1
@lstartClass1.set0
0;JMP
(loopendClass1.set0)
@ARG
D=M
@0
A=D+A
D=M
@SP
AM=M+1
A=A-1
M=D
@Class10
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@ARG
D=M
@1
A=D+A
D=M
@SP
AM=M+1
A=A-1
M=D
@Class11
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@0
D=A
@SP
AM=M+1
A=A-1
M=D
@LCL
D=M
@R13
M=D
@5
A=D-A
D=M
@R14
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13
M=M-1
A=M
D=M
@THAT
M=D
@R13
M=M-1
A=M
D=M
@THIS
M=D
@R13
M=M-1
A=M
D=M
@ARG
M=D
@R13
M=M-1
A=M
D=M
@LCL
M=D
@R14
A=M
0;JMP
(Class1.get)
@0
D=A
(lstartClass1.get0)
@loopendClass1.get0
D;JEQ
@SP
A=M
M=0
@SP
M=M+1
D=D-1
@lstartClass1.get0
0;JMP
(loopendClass1.get0)
@Class10
D=M
@SP
AM=M+1
A=A-1
M=D
@Class11
D=M
@SP
AM=M+1
A=A-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@LCL
D=M
@R13
M=D
@5
A=D-A
D=M
@R14
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13
M=M-1
A=M
D=M
@THAT
M=D
@R13
M=M-1
A=M
D=M
@THIS
M=D
@R13
M=M-1
A=M
D=M
@ARG
M=D
@R13
M=M-1
A=M
D=M
@LCL
M=D
@R14
A=M
0;JMP
(END)
@END
0;JMP

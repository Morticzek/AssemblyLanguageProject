.data
    EMPTY      db      " ",0
    extern strcmp: PROC

.code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;                 S T R U C T U R E S     S T A R T                          ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;struc vertex                                                                   ;;;;;
;;  vertexName          resq    1   ; name of the vertex                         ;;;;; +0
;;  previousVertexName  resq    1   ; name of the previous vertex                ;;;;; +8
;;  sumOfWeights        resd    1   ; sum of weights from the source             ;;;;; +16
;;  visited             resb    1   ; if this vertex has been visited            ;;;;; +20
;;  _padding1           resb    1   ; byte padding                               ;;;;; +22
;;  _padding2           resw    1   ; word padding                               ;;;;; +23
;;  nextPtrVertex       resq    1   ; next vertex pointer                        ;;;;; +24
;;endstruc                                                                       ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;struc connection                                                               ;;;;;
;;  sourceVertex        resq    1   ; name of the first vertex in the connection ;;;;; +0
;;  destinationVertex   resq    1   ; name of the second vertex in the connection;;;;; +8
;;  weight              resd    1   ; distance value between the vertices        ;;;;; +16
;;  _padding            resd    1   ; double word padding                        ;;;;; +20
;;  nextPtrConnection   resq    1   ; next connection pointer                    ;;;;; +24
;;endstruc                                                                       ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;                  S T R U C T U R E S     E N D                             ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; void mainProgram (Connection* headConnection, Vertex* headVertex, char* sourceVertex);;;;;
;; RCX - headConnection                                                                 ;;;;;
;; RDX - headVertex                                                                     ;;;;;
;; R8 - sourceVertex                                                                    ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main_program PROC
    push    rbp ; push base pointer on the stack
    mov     rbp, rsp ; copy stack pointer to the base pointer
    sub     rsp, 144 ; 32 for shadow space & 112 for variables stack expanding downwards
    mov     QWORD PTR [rbp-120], rcx ; headConnection on stack
    mov     QWORD PTR [rbp-128], rdx ; headVertex on stack
    mov     QWORD PTR [rbp-136], r8 ; sourceVertex on stack
    lea     rcx, [EMPTY] ; address of " "
    mov     QWORD PTR [rbp-112], rcx ; " " on stack
    mov     rax, QWORD PTR [rbp-128] ; headVertex becomes v
    mov     QWORD PTR [rbp-64], rax ; v
    jmp     vLoopTransition ; begin v while loop

vLoop:
    mov     rax, QWORD PTR [rbp-64] ; v
    mov     rax, QWORD PTR [rax] ; v->vertexName
    mov     r8, QWORD PTR [rbp-136] ; sourceVertex
    mov     rdx, r8 ; sourceVertex
    mov     rcx, rax ; *v (Vertex**)
    call    strcmp ; strcmp(headVertex->vertexName, sourceVertex)
    test    eax, eax ; set JE if ZR is set
    jne     vLoopNoMatch ; no match found
    mov     rax, QWORD PTR [rbp-64] ; headVertex
    mov     DWORD PTR [rax+16], 0 ; headVertex->sumOfWeights = 0
    jmp     vLoopMatch ; match found

vLoopNoMatch:
    mov     rax, QWORD PTR [rbp-64] ; v
    mov     rax, QWORD PTR [rax+24] ; v->nextPtr
    mov     QWORD PTR [rbp-64], rax ; v->nextPtr becomes v

vLoopTransition:
    cmp     QWORD PTR [rbp-64], 0 ; check if v exists
    jne     vLoop ; go to v1 loop / continue v loop

vLoopMatch:
    mov     rax, QWORD PTR [rbp-128] ; headVertex
    mov     QWORD PTR [rbp-72], rax ; headVertex becomes v1
    jmp     endPoint ; jump to endPoint

v1Loop:
    mov     rax, QWORD PTR [rbp-112] ; " "
    mov     QWORD PTR [rbp-48], rax ; minimumVertex = " "
    mov     DWORD PTR [rbp-36], 999999 ; minimumDistance = 999999
    mov     rax, QWORD PTR [rbp-112] ; " "
    mov     QWORD PTR [rbp-56], rax ; inspectedVertex = " "
    mov     rax, QWORD PTR [rbp-128] ; headVertex
    mov     QWORD PTR [rbp-80], rax ; v2 = headVertex
    jmp     v2LoopTransition ; begin v2 Loop

v1LoopNoMatch:
    mov     rax, QWORD PTR [rbp-80] ; v2
    movzx   eax, BYTE PTR [rax+20] ; v2->isVisited
    xor     eax, 1 ; check if isVisited == false
    test    al, al ; set JE if ZR is set
    je      v2LoopMatch ; no match found
    mov     rax, QWORD PTR [rbp-80] ; v2
    mov     eax, DWORD PTR [rax+16] ; v2->sumOfWeights
    mov     DWORD PTR [rbp-36], eax ; minimumDistance = v2->sumOfWeights
    jle     v2LoopMatch ; match found
    mov     rax, QWORD PTR [rbp-80] ; v2
    mov     eax, DWORD PTR [rax+16] ; v2->sumOfWeights
    mov     DWORD PTR [rbp-36], eax ; minimumDistance = v2->sumOfWeights
    mov     rax, QWORD PTR [rbp-80] ; v2
    mov     rax, QWORD PTR [rax] ; v2->vertexName
    mov     QWORD PTR [rbp-48], rax ; minimumVertex = v2->vertexName

v2LoopMatch:
    mov     rax, QWORD PTR [rbp-80] ; v2
    mov     rax, QWORD PTR [rax+24] ; v2->nextPtr
    mov     QWORD PTR [rbp-80], rax ; v2->nextPtr becomes v2

v2LoopTransition:
    cmp     QWORD PTR [rbp-80], 0 ; check if v2 exists
    jne     v1LoopNoMatch ; v2 exists
    mov     rax, QWORD PTR [rbp-48] ; minimumVertex
    cmp     rax, QWORD PTR [rbp-112] ; minimumVertex == " "
    je      v3LoopMatch ; minimumVertex is empty
    mov     rax, QWORD PTR [rbp-128] ; headVertex
    mov     QWORD PTR [rbp-88], rax ; v3 = headVertex
    jmp     v3LoopTransition ; begin v3 loop

v3Loop:
    mov     rax, QWORD PTR [rbp-88] ; v3
    mov     rax, QWORD PTR [rax] ; v3->vertexName
    mov     r8, QWORD PTR [rbp-48] ; minimumVertex
    mov     rdx, r8 ; minimumVertex
    mov     rcx, rax ; v3->vertexName
    call    strcmp ; strcmp(v3->vertexName, minimumVertex)
    test    eax, eax ; set JE if ZR is set
    jne     v3LoopNoMatch ; no match found
    mov     rax, QWORD PTR [rbp-88] ; v3
    mov     BYTE PTR [rax+20], 1 ; v3->isVisited = true
    jmp     v3LoopMatch ; match found

v3LoopNoMatch:
    mov     rax, QWORD PTR [rbp-88] ; v3
    mov     rax, QWORD PTR [rax+24] ; v3->nextPtr
    mov     QWORD PTR [rbp-88], rax ; v3->nextPtr becomes v3

v3LoopTransition:
    cmp     QWORD PTR [rbp-88], 0 ; check if v3 exists
    jne     v3Loop ; go to v3 loop / continue v3 loop

v3LoopMatch:
    mov     rax, QWORD PTR [rbp-120] ; headConnection
    mov     QWORD PTR [rbp-96], rax ; c = headConnection
    jmp     cLoopTransition ; move to cLoop

cLoop:
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax] ; c->sourceVertex
    mov     r8, QWORD PTR [rbp-48] ; minimumVertex
    mov     rdx, r8 ; minimumVertex
    mov     rcx, rax ; c->sourceVertex
    call    strcmp ; strcmp(c->sourceVertex, minimumVertex)
    test    eax, eax ; set JE if ZR is set
    je      cLoopMatchFirst ; match found
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax+8] ; c->destinationVertex
    mov     r8, QWORD PTR [rbp-48] ; minimumVertex
    mov     rdx, r8 ; minimumVertex
    mov     rcx, rax ; c->destinationVertex
    call    strcmp ; strcmp(c->destinationVertex, minimumVertex)
    test    eax, eax ; set JE if ZR is set
    jne     v4EndPoint ; match found


cLoopMatchFirst:
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax] ; c->sourceVertex
    mov     r8, QWORD PTR [rbp-48] ; minimumVertex
    mov     rdx, r8 ; minimumVertex
    mov     rcx, rax ; c->sourceVertex
    call    strcmp ; strcmp(c->sourceVertex, minimumVertex)
    test    eax, eax ; set JE if ZR is set
    jne     cLoopNoMatchFirst ; no match found
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax+8] ; c->destinationVertex
    mov     QWORD PTR [rbp-56], rax ; inspectedVertex = c->destinationVertex

cLoopNoMatchFirst:
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax+8] ; c->destinationVertex
    mov     r8, QWORD PTR [rbp-48] ; minimumVertex
    mov     rdx, r8 ; minimumVertex
    mov     rcx, rax ; c->destinationVertex
    call    strcmp ; strcmp(c->destinationVertex, minimumVertex)
    test    eax, eax; set JE if ZR is set
    jne     cLoopNoMatchSecond ; no match found
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax] ; c->sourceVertex
    mov     QWORD PTR [rbp-56], rax ; inspectedVertex = c->sourceVertex

cLoopNoMatchSecond:
    mov     rax, QWORD PTR [rbp-128] ; headVertex
    mov     QWORD PTR [rbp-104], rax ; v4 = headVertex
    jmp     v4LoopTransition ; begin v4 Loop

v4Loop:
    mov     rax, QWORD PTR [rbp-104] ; v4
    mov     rax, QWORD PTR [rax] ; v4->vertexName
    mov     r8, QWORD PTR [rbp-56] ; inspectedVertex
    mov     rdx, r8 ; inspectedVertex
    mov     rcx, rax ; v4->vertexName
    call    strcmp ; strcmp(v4->vertexName, inspectedVertex)
    test    eax, eax ; set JE if ZR is set
    jne     v4LoopNoMatch ; no match found
    mov     rax, QWORD PTR [rbp-104] ; v4
    mov     r8d, DWORD PTR [rax+16] ; v4->sumOfWeights
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     r9d, DWORD PTR [rax+16] ; c->distance
    mov     eax, DWORD PTR [rbp-36] ; minimumDistance
    add     eax, r9d ; minimumDistance + c->distance
    cmp     r8d, eax ; check if v4->sumOfWeights > minimumDistance + c->distance
    jle     v4LoopNoMatch ; no match found / sum is larger than v4->sumOfWeights
    mov     rax, QWORD PTR [rbp-104] ; v4
    mov     r8, QWORD PTR [rbp-48] ; minimumVertex
    mov     QWORD PTR [rax+8], r8 ; v4->previousVertexName = minimumVertex
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     r8d, DWORD PTR [rax+16] ; c->distance
    mov     eax, DWORD PTR [rbp-36] ; minimumDistance
    add     r8d, eax ; c->distance + minimumDistance
    mov     rax, QWORD PTR [rbp-104] ; v4
    mov     DWORD PTR [rax+16], r8d ; v4->sumOfWeights = c->distance + minimumDistance
    jmp     v4EndPoint ; end v4 Loop

v4LoopNoMatch:
    mov     rax, QWORD PTR [rbp-104] ; v4
    mov     rax, QWORD PTR [rax+24] ; v4->nextPtr
    mov     QWORD PTR [rbp-104], rax ; v4->nextPtr becomes v4

v4LoopTransition:
    cmp     QWORD PTR [rbp-104], 0 ; check if v4 exists
    jne     v4Loop ; begin v4 Loop

v4EndPoint:
    mov     rax, QWORD PTR [rbp-96] ; c
    mov     rax, QWORD PTR [rax+24] ; c->nextPtr
    mov     QWORD PTR [rbp-96], rax ; c->nextPtr becomes c

cLoopTransition:
    cmp     QWORD PTR [rbp-96], 0 ; check if c exists
    jne     cLoop ; begin c Loop
    mov     rax, QWORD PTR [rbp-72] ; v1
    mov     rax, QWORD PTR [rax+24] ; v1->nextPtr
    mov     QWORD PTR [rbp-72], rax ; v1->nextPtr becomes v1

endPoint:
    cmp     QWORD PTR [rbp-72], 0 ; check if v1 exists
    jne     v1Loop ; go to v1 Loop if it exists
    jmp     terminateProgram

terminateProgram:
    add     rsp, 144
    pop     rbp
    ret

main_program endp
end
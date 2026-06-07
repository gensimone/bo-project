# SBO-RCE (Stack Buffer Overfow with Remote Code Execution).
---

## Obiettivo.
Ottenere una shell su un sistema target (denominato TARGET nel resto del documento)
vulnerabile ad un attacco di buffer overflow. In particolare, la vulnerabilita'
studiata (e sfruttata), permettera' di ottenere un buffer overflow ed esecuzione
di codice remoto (RCE) nello stack.
---

## Requisiti.
Per raggiungere l'obiettivo preposto e' necessario che TARGET soddisfi i seguenti
requisiti:
(1) Il sistema operativo di TARGET deve permettere l'esecuzione di codice nello stack.
    - return-to-lib attack.

(2) TARGET deve eseguire codice vulnerabile ad un attacco buffer overflow.
(3) Il software vulnerabile ad un attacco buffer overflow in TARGET non deve eseguire
    controlli sulla integrita' dello stack (stack protector).
(4) Il buffer in cui si verifica l'overflow deve avere capienza sufficiente a contenere
    il payload.
(5) ASLR (Address space layout randomization) - NOP SLIDER.

## Implementazione
### Requisito 1 - OS Vulnerabile.
Generalmente i sistemi operativi moderni non permettono l'esecuzione di codice nello
stack. Tuttavia, effettuando delle opportune modifiche a file di sistema o ricompilando
il kernel del sistema operativo, e' possibile disabilitare questa misura di sicurezza.
In FreeBSD e' sufficiente eseguire il seguente comando:
```sh
sysctl kern.elf64.nxstack=0
```
Tale comando ha l'effetto di permettere l'esecuzione di codice nello stack per eseguibili
a 64 bit.
Un'altra misura

Per ragioni di semplicita' FreeBSD v15.0 e' stato scelto come OS per TARGET.
---

### Requisito 2 - Codice vulnerabile.

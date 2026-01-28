# Laboratório de Sistemas Operacionais 🛠️

Este repositório contém uma coleção de implementações de conceitos fundamentais de Sistemas Operacionais realizados durante o semestre 2025.2.

## 📁 Conteúdo do Repositório

### 1. [Mini-Shell](./01-Mini_Shell) 🐧
Um interpretador de comandos, desenvolvido em C, que interage diretamente com as system calls do Kernel do Linux.
- **Principais Funcionalidades:**
  - Execução de binários do sistema em primeiro e segundo plano (background processes).
  - Comandos internos: `jobs` (listar processos ativos), `pid` (identificar processos), `wait` (sincronização) e `exit`.
  - Gerenciamento automático de processos terminados para evitar estados zumbi.
- **Conceitos de SO aplicados:**
  - Criação de processos com `fork().`
  - Substituição de imagem de processo com `execvp()`.
  - Sincronização e monitoramento com `waitpid()` e `WNOHANG`.
  - Tratamento de sinais básicos.

### 2. [Simulador de Gerência de Memória](./03-Gerencia_de_Memoria) 🧠

Um simulador desenvolvido em Python para analisar o comportamento de diferentes algoritmos de alocação em partições dinâmicas de memória.

- **Estratégias de Alocação:**
  - First Fit: Alocação no primeiro bloco disponível.
  - Best Fit: Busca pelo bloco que deixa o menor resíduo.
  - Worst Fit: Alocação no maior bloco para manter sobras úteis.

- **Conceitos de SO aplicados:**
  - Fragmentação Externa: Monitoramento de buracos na memória física.
  - Coalescência: Fusão automática de blocos livres adjacentes após a liberação.
  - Split de Blocos: Divisão dinâmica de partições durante a alocação.
  - Estatísticas de Uso: Cálculo de uso efetivo e métricas de ocupação.
---
## 🚀 Como testar
Cada diretório possui seu próprio `Makefile`. Para testar qualquer uma das tarefas, navegue até a pasta e execute:
```bash
make
make run

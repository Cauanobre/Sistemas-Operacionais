FIRST_FIT = "first"
BEST_FIT = "best"
WORST_FIT = "worst"

memory = []                 # Lista de blocos (ordem física)
total_memory_size = 0
next_process_id = 1



# Inicialização

def mem_init(size):
    global memory, total_memory_size, next_process_id

    memory = [{
        "id": 0,
        "start": 0,
        "size": size,
        "free": True
    }]

    total_memory_size = size
    next_process_id = 1
    print(f"Memoria inicializada com {size} bytes.")



# Escolha do bloco (NÃO altera memória)

def choose_block(size, alg):
    candidates = [bloco for bloco in memory if bloco["free"] and bloco["size"] >= size]

    if not candidates:
        return None

    if alg == FIRST_FIT:
        return candidates[0]

    if alg == BEST_FIT:
        return min(candidates, key=lambda b: b["size"] - size)

    if alg == WORST_FIT:
        return max(candidates, key=lambda b: b["size"])

    return None



# Alocação

def mem_alloc(size, alg):
    global next_process_id

    if not memory:
        print("Erro: Memoria nao inicializada. Use 'init'.")
        return

    if alg not in (FIRST_FIT, BEST_FIT, WORST_FIT):
        print("Algoritmo desconhecido.")
        return

    target = choose_block(size, alg)

    if target is None:
        print(f"Erro: Nao ha espaco suficiente para alocar {size} bytes ({alg}).")
        return

    pid = next_process_id
    next_process_id += 1

    idx = memory.index(target)

    # Cria bloco alocado
    allocated = {
        "id": pid,
        "start": target["start"],
        "size": size,
        "free": False
    }

    memory[idx] = allocated

    # Split se houver sobra
    if target["size"] > size:
        free_block = {
            "id": 0,
            "start": target["start"] + size,
            "size": target["size"] - size,
            "free": True
        }
        memory.insert(idx + 1, free_block)

    print(f"Bloco alocado: ID={pid}, Tamanho={size}, Endereco={allocated['start']}")



# Liberação + Coalescência

def mem_free(block_id):
    for i, block in enumerate(memory):
        if not block["free"] and block["id"] == block_id:
            block["free"] = True
            block["id"] = 0
            print(f"Bloco ID={block_id} liberado.")
            coalesce()
            return

    print(f"Erro: ID {block_id} nao encontrado ou ja liberado.")


def coalesce():
    global memory

    i = 0
    while i < len(memory) - 1:
        curr = memory[i]
        nxt = memory[i + 1]

        if curr["free"] and nxt["free"]:
            curr["size"] += nxt["size"]
            memory.pop(i + 1)
        else:
            i += 1



# Mapa de Memória

def mem_show():
    print(f"\nMapa de Memoria ({total_memory_size} bytes)")
    print("-" * 60)

    # Linha 1: uso físico
    print("[", end="")
    for b in memory:
        print("#" * b["size"] if not b["free"] else "." * b["size"], end="")
    print("]")

    # Linha 2: IDs
    print("[", end="")
    for b in memory:
        if b["free"]:
            print("." * b["size"], end="")
        else:
            print(str(b["id"] % 10) * b["size"], end="")
    print("]")
    print("-" * 60)

    print("Blocos ativos: ", end="")
    ativos = [
        f"[id={b['id']}] @{b['start']} +{b['size']}B (usado={b['size']}B)"
        for b in memory if not b["free"]
    ]
    print(" | ".join(ativos) if ativos else "nenhum")



# Estatísticas

def mem_stats():
    occupied = sum(b["size"] for b in memory if not b["free"])
    free_space = sum(b["size"] for b in memory if b["free"])
    holes = sum(1 for b in memory if b["free"])
    int_frag = 0

    usage = (occupied / total_memory_size) * 100 if total_memory_size else 0

    print("== Estatisticas ==")
    print(f"Tamanho total: {total_memory_size} bytes")
    print(f"Ocupado: {occupied} bytes | Livre: {free_space} bytes")
    print(f"Buracos (fragmentacao externa): {holes}")
    print(f"Fragmentacao interna: {int_frag} bytes")
    print(f"Uso efetivo: {usage:.2f}%")



# CLI
def main():
    print("=== Simulador de Gerencia de Memoria ===")
    print("Comandos: init <tam>, alloc <tam> <algo>, freeid <id>, show, stats, exit\n")

    while True:
        try:
            cmd = input("> ").strip().split()

            if not cmd:
                continue

            if cmd[0] == "exit":
                break

            elif cmd[0] == "init":
                mem_init(int(cmd[1]))

            elif cmd[0] == "alloc":
                mem_alloc(int(cmd[1]), cmd[2])

            elif cmd[0] == "freeid":
                mem_free(int(cmd[1]))

            elif cmd[0] == "show":
                mem_show()
                mem_stats()

            elif cmd[0] == "stats":
                mem_stats()

            else:
                print("Comando invalido.")

        except Exception as e:
            print("Erro:", e)


if __name__ == "__main__":
    main()
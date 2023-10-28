import os, time
import matplotlib.pyplot as plt

PROGRESS = 0
SIZE = 100
TENTATIVAS = 1

def progress():
    global PROGRESS, TENTATIVAS

    PROGRESS += 100/(TENTATIVAS*6)
    porcentagem = round(PROGRESS)
    print(f"Carregando resultados: {porcentagem} %", end="\r")

def calcTimeSerial(tries: int, size: int) -> float:
    media = 0
    for i in range(tries):
        if not(KeyboardInterrupt):
            print("O programa foi abortado!")
            exit()

        t0 = time.time()
        os.system(f"./a.out {size}")
        media += time.time()-t0

        progress()
    
    return 1000*media/tries

def calcTime(tries: int, size: int, num_processos: int) -> float:
    media = 0
    for i in range(tries):
        if not(KeyboardInterrupt):
            print("O programa foi abortado!")
            exit()

        t0 = time.time()
        os.system(f"mpirun --oversubscribe -np {num_processos} a.out {size}")
        media += time.time()-t0

        progress()

    return 1000*media/tries

def showResults(valores: list, categorias: list) -> None:

    global SIZE, TENTATIVAS

    # Primeiro grafico
    plt.figure(1)
    plt.bar(categorias, valores, color='royalblue', edgecolor='black')

    # Adicione rótulos e título
    plt.xlabel('Número de Processos MPI', fontsize=12)
    plt.ylabel('Tempo (ms)', fontsize=12)
    plt.title('Desempenho MPI', fontsize=14)

    plt.text(0, max(valores)*1.1, f"Tentativas: {TENTATIVAS}\nMatriz: {SIZE}X{SIZE}")

    # Defina o tamanho da fonte dos rótulos nos eixos
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)

    # Adicione uma grade de fundo
    plt.grid(linestyle='--', alpha=0.5)

    # Ajuste os limites dos eixos
    plt.ylim(0, max(valores)*1.3)

    # Segundo grafico
    plt.figure(2)
    speedup = [round(((valores[0]/valor) - 1)*100, 2) for valor in valores]
    plt.bar(categorias, speedup, color='green', alpha=0.5, edgecolor='black')

    # Defina o tamanho da fonte dos rótulos nos eixos
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)

    # Adicione uma grade de fundo
    plt.grid(linestyle='--', alpha=0.5)

    plt.xlabel('Número de Processos MPI')
    plt.ylabel('Speedup (%)')
    plt.title('Speedup em Relação ao Número de Processos')

    # Mostre o gráfico
    plt.show()

def main() -> None:

    global SIZE, TENTATIVAS
    print(f"Carregando resultados: 0 %", end="\r")
    os.system("g++ multiplicacao_normal.cpp")
    valores = [calcTimeSerial(TENTATIVAS, SIZE)]


    #os.system("mpic++ multiplicacao.cpp ")
    #os.system("mpic++ gpt.cpp ")
    #os.system("mpic++ github.cpp ")
    os.system("mpic++ github_scatter.cpp ")

    # Dados para o gráfico de barras
    num_processos = ['0', '1', '2', '4', '8', '10']
    valores.extend([calcTime(TENTATIVAS, SIZE, int(num_processos[i+1])) for i in range(len(num_processos)-1)])

    showResults(valores, num_processos)


main()

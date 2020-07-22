import subprocess

def main():
    numTries = "10"
    results = open("results.txt", "w")
    errorFile = open("error.txt", "w")

    for x in [1, 2, 4, 8]:
        for y in [1, 10, 100, 1000, 10000, 100000, 500000]:
            numThreads = str(x)
            numTrials = str(y)
            subprocess.call(["Project1.exe", numThreads, numTries, numTrials], stdout=results, stderr=errorFile, shell=True)

    results.close()
    errorFile.close()

if __name__ == "__main__":
    main()
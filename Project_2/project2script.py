
#C:\Users\Lachlan\Documents\Visual Studio 2017\Projects\parallel\Project1\Debug

import subprocess

def main():
    results = open("results.txt", "w")
    errorFile = open("error.txt", "w")

    for x in [1, 2, 4, 8]:
        for y in [1, 5, 10, 20, 40, 80, 160, 320, 640, 1280]:
            numThreads = str(x)
            numNodes = str(y)
            subprocess.call(["Project2.exe", numThreads, numNodes], stdout=results, stderr=errorFile, shell=True)

    results.close()
    errorFile.close()

if __name__ == "__main__":
    main()
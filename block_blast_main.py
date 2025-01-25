import subprocess
def main() -> None:
    C_EXECUTABLE : str = "add.exe"
    X : str = "18"
    Y : str = "5"
    OUTPUT = subprocess.run([C_EXECUTABLE, X, Y], capture_output=True, text=True, check=True)
    RESULT = OUTPUT.stdout
    print(f"{X} + {Y} = {RESULT}")

if __name__ == "__main__":
    main()
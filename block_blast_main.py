import subprocess
from time import sleep
from os import system

# TODO: get_best_moves(blocks) -> C++
# TODO: update_board, display_board, display_best_moves.
class BlockBlast:

    def __init__(self) -> None:
        self.board_size : int = 8
        self.is_filled : str = 'o'
        self.is_empty : str = '.'
        self.board : list[list[int]] = [[0 for _ in range(self.board_size)] for _ in range(self.board_size)]
        self.current_blocks : list[list[list[int]]] = []

    def start(self) -> None:
        # DEV ONLY: Get latest version of executable.
        system("g++ blockBlastDepthFirstSearch.cpp -o blockBlastDepthFirstSearch.exe")

        # MAIN
        self.setup_initial_config()
        while True:
            # Input section.
            Utils.clear_console()
            Utils.display_board(b.board)
            blocks : list[list[list[int]]] = Utils.get_user_blocks()
            best_moves : list[list[list[int]]] = self.get_best_moves(blocks)
            
            # Output section.
            Utils.clear_console()
            self.update_board(best_moves)
            Utils.display_board(b.board)
            self.display_best_moves(best_moves)

            # Wait for user confirmation to continue.
            Utils.pause()
            
            
    def setup_initial_config(self) -> None:
        if input("Has initial board config? (Y/n): ") in ["Y", "y"]:

            # 9 to allow for one more line (empty input) to exit.
            self.board = Utils.get_block_config(run_for=9)

    def get_best_moves(self, blocks : list[list[list[int]]]) -> list[list[list[int]]]:
        C_EXEC : str = "blockBlastDepthFirstSearch.exe"
        try:
            PROCESS : subprocess.CompletedProcess[bytes] = subprocess.run([C_EXEC, 
                                    Utils.get_string_representation_of_array(self.board)] +
                                    [Utils.get_string_representation_of_array(block) for block in blocks],
                                    capture_output=True)
            PROCESS_OUTPUT : str = PROCESS.stdout.decode()
            best_moves : list[list[int]] = Utils.parse_moves(PROCESS_OUTPUT)
            Utils.pause()
        except subprocess.SubprocessError as e:
            print(f"subprocess_error : {e}")
            Utils.pause()
        except subprocess.CalledProcessError as e:
            print(f"calledprocess_error : {e}")
            Utils.pause()
        except FileNotFoundError as e:
            print(f"Program cannot find DFS executable.")
            Utils.pause()

    def display_best_moves(self, best_moves : list[list[list[int]]]) -> None:
        pass
    def update_board(self, best_moves : list[list[list[int]]]) -> None:
        pass

    
class Utils:
    board_size : int = 8

    @staticmethod
    def get_block_config(run_for=9, is_filled='o', is_empty='.') -> list[list[int]]:
        '''Gets a single 8x8 max "block" from the user, and returns a binary array representation of it.'''
        block_config : list[list[int]] = []

        # For 'run_for' times.
        for i in range(run_for):
            current_row : list[int] = []

            # Get each row.
            while True:

                user_input : str = input(f"Line {i + 1} >> ")
                if (user_input != "" and 
                    all([True if char in [is_filled, is_empty] else False for char in user_input]) and
                    len(user_input) <= Utils.board_size): # Valid.
                    current_row = [1 if char == is_filled else 0 for char in user_input]
                    break
                elif user_input == "": # Finish.
                    return block_config
                else: # Invalid.
                    continue

            # Append each row. But only if it isn't empty.
            if bool(current_row):
                block_config.append(current_row)

    @staticmethod
    def get_user_blocks() -> list[list[list[int]]]:
        '''Returns a 3D array for all three 2D array "blocks".'''
        print("\nINSTRUC: Enter [o] for filled cells, [.] for empty cells. Enter nothing to end input.")
        blocks : list[list[list[int]]] = []
        for i in range(3):
            print(f"\nBlock {i + 1}:\n")
            blocks.append(Utils.get_block_config())
        return blocks

    @staticmethod
    def display_board(board: list[list[int]]) -> None:
        for row in board:
            for cell in row:
                print(f"{"◼" if cell else " "} ", end="")
            print()

    @staticmethod
    def get_string_representation_of_array(array : list[list[int]], is_filled='o', is_empty='.'):
        return_string : str = ""
        for i, row in enumerate(array):
            for cell in row:
                return_string += f"{is_filled if cell else is_empty}"
            if i != len(array) - 1: 
                return_string += "_"
        return return_string
    
    @staticmethod
    def parse_moves(stdout : str) -> list[list[int]]:
        print("#FROM PYTHON#")
        print(stdout)
        pass
    @staticmethod
    def pause() -> None:
        input("Press Enter to continue...")

    @staticmethod
    def clear_console() -> None:
        system("cls")
if __name__ == "__main__":
    b = BlockBlast()
    b.start()
        


    
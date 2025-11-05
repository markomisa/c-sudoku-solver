#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include all our custom headers
#include "tree.h"
#include "queue.h"
#include "sudoku.h"
#include "utils.h"

int main()
{
    int **rootTable;
    int **currentTableState;
    int unos; // 'unos' means 'input', kept variable name for logic consistency
    int row, col, val;
    TreeNode *root, *currentNode;

    printf("\n-----Sudoku-----\n");
    printf("\nAre you using a file or entering the board manually?\n");
    printf("1. Manual input\n");
    printf("2. File\n");
    int n = 4; // Default to 4x4
    printf("\nSelect an option: ");
    scanf("%d", &unos);

    if (unos == 2)
    {
        char ime[] = "sudoku4x4_3.txt"; // Hardcoded filename
        n = 4; 
        rootTable = readMatrixFile(n, ime);
    }
    else if (unos == 1)
    {
        printf("\nEnter the size, and then enter the sudoku fields\n");
        printf("\nSize: ");
        scanf("%d", &n);
        printf("Enter %d x %d numbers (0 for empty):\n", n, n);
        rootTable = readMatrix(n);
    } else {
        printf("Invalid input.\n");
        return 1;
    }

    // This solver assumes n=4. Warn user if n is different.
    if (n != 4) {
        printf("\nWARNING: This solver is optimized for 4x4.\n");
        printf("Validation and solving might not work correctly for %dx%d.\n", n, n);
    }
            
    currentTableState = rootTable;
    root = createTreeNode(copyMatrix(currentTableState, n), 0, 0);
    printf("Generating solution tree... ");
    makeTree(root);
    printf("Done.\n");

    while (true)
    {
        printf("\n-----------------------------------------------------\n");
        printf("Current board: \n");
        printMatrix(currentTableState, n);
        printf("-----------------------------------------------------\n");
        printf("\n1. Print the generated tree\n");
        printf("2. Print all possible solutions (if any)\n");
        printf("3. Enter a number into the sudoku\n");
        printf("4. Check the current state of the board\n");
        printf("5. Check if the current state leads to a solution\n");
        printf("6. Get a hint for the next move\n");
        printf("7. I claim the sudoku has no solution\n");
        printf("8. End game\n");
        printf("\n-----------------------------------------------------\n");
        printf("\nSelect an option (enter a number from 1-8): ");
        scanf("%d", &unos);

        switch (unos)
        {
        case 1:
            levelOrderTraversal(root);
            break;
        case 2:
            writeSolutions(root); // Note: This function's prints are in sudoku.c
            break;
        case 3:
            if (countZero(currentTableState, n) == 0) {
                 printf("\nThe board is already full!\n");
                 break;
            }
            
            printf("\n-----------------------------------------------------\n");
            printf("\nEnter row number (1 - %d), column number (1 - %d), and value (1 - %d): \n", n, n, n);
            printf("             (row)              (column)\n");
            printf("\n-----------------------------------------------------\n");
            scanf("%d%d%d", &row, &col, &val);
            row--; // Adjust for 0-based index
            col--; 

            // Basic bounds checking
            if (row < 0 || row >= n || col < 0 || col >= n || val < 1 || val > n) {
                printf("\nInvalid input. Please try again.\n");
                break;
            }

            if (currentTableState[row][col] == 0)
            {
                currentTableState[row][col] = val;
                if (!isValid(currentTableState, n))
                {
                    printf("\n-----------------------------------------------------\n");
                    printf("\nYou cannot place that number there! (Move is not valid)\n");
                    currentTableState[row][col] = 0; // Revert move
                    break;
                }
            }
            else
            {
                printf("\n-----------------------------------------------------\n");
                printf("You cannot enter a number there (field is not empty)!\n");
                break;
            }

            // Check for win
            if (countZero(currentTableState, n) == 0) {
                if (isValid(currentTableState, n)) {
                    printMatrix(currentTableState, n);
                    printf("\nCongratulations, you won!\n");
                    printf("\nYou have finished the game!\n");
                    freeTree(root);
                    freeMatrix(currentTableState, n); // currentTableState is rootTable
                    exit(0);
                } else {
                    printf("\nThe board is full, but not correct!\n");
                }
            }

            // Rebuild tree from new state
            printf("Updating solution tree... ");
            freeTree(root);
            root = createTreeNode(copyMatrix(currentTableState, n), 0, 0);
            makeTree(root);
            printf("Done.\n");
            break;
        case 4:
            if (isValid(currentTableState, n))
            {
                printf("\n-----------------------------------------------------\n");
                printf("\nThe current state of the board is valid\n");
            }
            else
            {
                printf("\n-----------------------------------------------------\n");
                printf("\nThe current state of the board is NOT valid\n");
            }
            break;
        case 5:
        { // Braces needed for new variable declaration in switch
            printf("Checking for solutions... ");
            TreeNode *tempRoot = createTreeNode(copyMatrix(currentTableState, n), 0, 0);
            makeTree(tempRoot);
            if (isThereSolutions(tempRoot))
            {
                printf("\n-----------------------------------------------------\n");
                printf("\nSolutions exist for this board\n");
            }
            else
            {
                printf("\n-----------------------------------------------------\n");
                printf("\nNo solutions exist for this board\n");
            }
            freeTree(tempRoot);
            break;
        }
        case 6:
            printf("\nHINT:\n");
            // Tree must be current
            if (root->depth != 0 || countZero(root->table, n) != countZero(currentTableState, n)) {
                 printf("The tree is not up-to-date, updating...\n");
                 freeTree(root);
                 root = createTreeNode(copyMatrix(currentTableState, n), 0, 0);
                 makeTree(root);
            }
            currentNode = giveHint(root); // Note: This function's prints are in sudoku.c
            if (currentNode != NULL)
                printMatrix(currentNode->table, n);
            break;
        case 7:
            if (isThereSolutions(root))
            {
                printf("\n-----------------------------------------------------\n");
                printf("\nThe sudoku has solutions! Your answer is incorrect.\n");
                break;
            }
            else
            {
                printf("\n-----------------------------------------------------\n");
                printf("\nCorrect! The sudoku has no solution!\n");
                printf("\nYou have finished the game!\n");
                freeTree(root);
                freeMatrix(currentTableState, n); // currentTableState is rootTable
                exit(0);
            }
        case 8:
            printf("\nYou have finished the game!\n");
            freeTree(root);
            freeMatrix(currentTableState, n); // currentTableState is rootTable
            exit(0);
        default:
            printf("\nUnknown option. Please enter a number from 1-8.\n");
            break;
        }
    }
    return 0;
}
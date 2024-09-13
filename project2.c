#include "project2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024


int main(int argc, char *argv[])
{
	// Initialize this pipeline simulator //
    Simple_Pipe simple_pipe;
    for (int i = 0; i < REG_COUNT; i++)
    {
        simple_pipe.registers[i] = 0;
    }

    simple_pipe.setNum = 0;
    simple_pipe.addNum = 0;
    simple_pipe.subNum = 0;
    simple_pipe.mulNum = 0;
    simple_pipe.divNum = 0;
    simple_pipe.ldNum = 0;
    simple_pipe.retNum = 0;

	execution_time = 0;
	instruction_count =0;


    
	// Model your pipeline simulator //
     if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1; // Return an error code
    }
    FILE *file;
    char const *filename=argv[1];
    char **lines;  
    char buffer[MAX_LINE_LENGTH];
    int lineCount = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }

    fseek(file, 0, SEEK_SET);
    lines = (char **)malloc(lineCount * sizeof(char *));
    if (lines == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }
    for (int i = 0; i < lineCount; i++) {
    instruction_count++;
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        lines[i] = (char *)malloc(strlen(buffer) + 1);
        if (lines[i] == NULL) {
            perror("Memory allocation error");
            fclose(file);

            for (int j = 0; j < i; j++) {
                free(lines[j]);
            }
            free(lines);

            return 1;
        }

        strcpy(lines[i], buffer);
        char firstBits[9]; 
        strncpy(firstBits, lines[i], 8);
        firstBits[8] = '\0';
        long decimalValue = strtol(firstBits, NULL, 2);
        
        if (decimalValue == 0x60) {
            simple_pipe.retNum++;
            execution_time++;
            break;
           
        }

        char rx[9];
        strncpy(rx, lines[i]  + 8, 8); 
        rx[8] = '\0';
        long RX = strtol(rx, NULL, 2);

        char ry[9];
        strncpy(ry, lines[i] + 16 , 8);  
        ry[8] = '\0';
        long RY = strtol(ry, NULL, 2);

        char rz[9];
        strncpy(rz, lines[i] + 24, 8); 
        rz[8] = '\0';
        long RZ = strtol(rz, NULL, 2);

        char imm[9];
        strncpy(imm, lines[i] + 24, 8);
        imm[8] = '\0';
        long Imm = strtol(imm, NULL, 2);

        char imm1[17];
        strncpy(imm1, lines[i] + 16, 16);
        imm1[16] = '\0';
        long Imm1 = strtol(imm1, NULL, 2);

            if(decimalValue == 0){
                simple_pipe.registers[RX] = Imm1;
                simple_pipe.setNum++;
                execution_time++;

            }
            else if(decimalValue == 0x10){
                simple_pipe.registers[RX] = simple_pipe.registers[RY]+ simple_pipe.registers[RZ];
                simple_pipe.addNum++;
                execution_time++;
            }
            else if(decimalValue == 0x11){
                simple_pipe.registers[RX] = simple_pipe.registers[RY]+ Imm;
                simple_pipe.addNum++;
                execution_time++;
            }
            else if(decimalValue == 0x20){
                simple_pipe.registers[RX] = simple_pipe.registers[RY] - simple_pipe.registers[RZ];
                simple_pipe.subNum++;
                execution_time++;
            }
            else if(decimalValue == 0x21){
                simple_pipe.registers[RX] = simple_pipe.registers[RY] - Imm;
                simple_pipe.subNum++;
                execution_time++;
            }
            else if(decimalValue == 0x30) {
                simple_pipe.registers[RX] = simple_pipe.registers[RY] * simple_pipe.registers[RZ];
                simple_pipe.mulNum++;
                execution_time++;
            }
            else if(decimalValue == 0x31) {
                simple_pipe.registers[RX] = simple_pipe.registers[RY] * Imm;
                simple_pipe.mulNum++;
                execution_time++;
            }
             else if(decimalValue == 0x40) {
                simple_pipe.registers[RX] = simple_pipe.registers[RY] / simple_pipe.registers[RZ];
                simple_pipe.divNum++;
                execution_time++;
            }
            else if(decimalValue == 0x41) {
                simple_pipe.registers[RX] = simple_pipe.registers[RY] / Imm;
                simple_pipe.divNum++;
                execution_time++;
            }     
            else if(decimalValue == 0x50) {

                FILE *ptr1;
                ptr1 = fopen(filename, "r");
                char line[1000];
                int count = 0;
                char str[1000];
                int value;

                while (fgets(str, sizeof(line), ptr1) != NULL) {
                    if (count == Imm1/4) {
                        value = atoi(str);
                        long Value = strtol(str, NULL, 2);
                        simple_pipe.registers[RX] = Value;
                        simple_pipe.ldNum++;
                        execution_time++;
                        execution_time++;
                        break;
                    }
                count++;
                }

            fclose(ptr1);       
            }
            else if (decimalValue == 0x51) {
                int ryregister = simple_pipe.registers[RY];
                int rzregister = simple_pipe.registers[RZ];
                int addition = (ryregister + rzregister) / 4;
                FILE *ptr2;
                ptr2 = fopen(filename, "r");
                char line[1000];
                int count = 0;
                char str[1000];
                int value;

                while (fgets(str, sizeof(line), ptr2) != NULL) {
                    if (count == addition) {
                        value = atoi(str);
                        long Value = strtol(str, NULL, 2);
                        simple_pipe.registers[RX] = Value;
                        simple_pipe.ldNum++;
                        execution_time++;
                        execution_time++;
                        break;
                    }
                count++;
                }

            fclose(ptr2);
            }
        }
    }
    for (int i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);

    fclose(file);

	// Print your simulation results //
	print_regs(simple_pipe);

    printf("Number of instruction SET: %d\n", simple_pipe.setNum);
    printf("Number of instruction ADD: %d\n", simple_pipe.addNum);
    printf("Number of instruction SUB: %d\n", simple_pipe.subNum);
    printf("Number of instruction MUL: %d\n", simple_pipe.mulNum);
    printf("Number of instruction DIV: %d\n", simple_pipe.divNum);
    printf("Number of instruction LD: %d\n", simple_pipe.ldNum);
    printf("Number of instruction RET: %d\n", simple_pipe.retNum);
    printf("\nTotal execution cycles: %d\n", execution_time);
	if (execution_time > 0)
	    printf("IPC: %f\n", (instruction_count/(double)execution_time));
    printf("\n");
    return 0;
}

void print_regs(Simple_Pipe simple_pipe)
{
    printf("\nRegisters: \n");
    printf("----------------------------------------\n");
    for (int i = 0; i < REG_COUNT; i += 2)
    {
        if (i < 31)
        {
            if (i < 10)
            {
                printf("  R%d: %-10d |   R%-5d: %-10d\n", i, simple_pipe.registers[i], i + 1, simple_pipe.registers[i + 1]);
            }
            else
            {
                printf("  R%d: %-9d |   R%-5d: %-9d\n", i, simple_pipe.registers[i], i + 1, simple_pipe.registers[i + 1]);
            }

            printf("----------------------------------------\n");
        }
        else
        {
            printf("----------------------------------------\n");
        }
    }
    printf("\n");
}
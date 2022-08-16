#include <stdio.h>
#include <string.h>
#include <time.h>
#define NR 20
#define NR_ROOMS 999
#define RED "\033[31m"

int run = 1;
int number_reserved = 0;

/* Struct of chairs */
struct chair {
  int id;
  int status; // 0 => free, 1 => taken 
  int room;
  time_t time_taken;
  int weight;
};

struct chair chairs[NR];

void init_chairs() {
  for (int i = 0; i < NR; i++) {
    chairs[i].id = i;
    chairs[i].status = 0;
    chairs[i].time_taken = 0;
    chairs[i].weight = 0;
    chairs[i].room = 0;
  }
}

/* Struct of rooms */
struct room {
  int nr_persons;
};

struct room rooms[NR_ROOMS];

void init_rooms() {
  for (int i = 0; i < NR_ROOMS; i++) {
    if (i < 200) {
      rooms[i].nr_persons = 1;
    } else if (i < 300) {
      rooms[i].nr_persons = 2;
    } else if (i < 400) {
      rooms[i].nr_persons = 3;
    } else if (i < 500) {
      rooms[i].nr_persons = 4;
    } else if (i < 600) {
      rooms[i].nr_persons = 5;
    } else if (i < 700) {
      rooms[i].nr_persons = 6;
    } else if (i < 800) {
      rooms[i].nr_persons = 7;
    } else if (i < 900) {
      rooms[i].nr_persons = 8;
    } else {
      rooms[i].nr_persons = 9;
    }
  }
}


/* Loop Funtions */

void add_time(int index) {
  chairs[index].time_taken = time(NULL);
}

int get_free_id() {
  int result;
  for (int i = 0; i < NR; i++) {
    if (chairs[i].status == 0) {
      result = chairs[i].id;
      break;
    }
  }
  return result;
}

void reserve_chairs() {
  int input;
  printf("How many chairs would you like to reserve?\n");
  scanf(" %d", &input);

  if (0 >= input) {
    printf("Not a valid input\n");
  } else if (number_reserved + input > NR) {
    printf("Not enough free chairs.\n");
  } else {
    int temp_room;
    printf("On what room?\n");
    scanf(" %d", &temp_room);

    if (0 > temp_room || temp_room > NR_ROOMS) {
      printf("This room does not exist\n");
    } else if (input > rooms[temp_room].nr_persons) {
      printf("Your room can only reserve %d chairs.\n", rooms[temp_room].nr_persons);
    } else {
      int id_array[input - 1];
      int flag = 1; // 1 not done, 0 done

      for (int i = 0; i < NR - input + 1; i++) {
        if (flag == 1) {
          for (int index = 0; index < input; index++) {
            if (chairs[i + index].status == 0) {
              id_array[index] = chairs[i + index].id;
              if (index + 1 == input) {
                flag = 0;
              }
            } else {
              break;
            }
          }
        } else {
          break;
        }
      }

      if (flag == 1) {
        char cmd;
        printf("Did not find %d consecutive chairs.\n", input);
        printf("Do you still want to reserve these chairs?\n");
        scanf(" %c", &cmd);

        if (cmd == 'y') {
          printf("Your chairs are:");
          for (int i = 0; i < input; i++) {
            int temp_id = get_free_id();
            chairs[temp_id].status = 1;
            chairs[temp_id].room = temp_room;
            add_time(temp_id);
            if (i != 0) {
              printf(",");
            }
            printf(" %d", temp_id);
          }
          printf("\n\n");
          number_reserved+= input;
        }

      } else {
        printf("Found %d consecutive chairs.\nWith numbers:", input);
        for (int i = 0; i < input; i++) {
          int temp_id = id_array[i];
          chairs[temp_id].status = 1;
          chairs[temp_id].room = temp_room;
          add_time(temp_id);
          number_reserved++;
          if (i != 0) {
            printf(",");
          }
          printf(" %d", temp_id);
        }
        printf("\n\n");
      }
    }
  }
}

void free_chairs() {
  int input;
  printf("To what room are you chairs linked?\n");
  scanf(" %d", &input);

  if (0 > input || input > NR_ROOMS) {
    printf("This room does not exist\n");
  } else {
    for (int i = 0; i < NR; i++) {
      if (chairs[i].room == input) {
        chairs[i].status = 0;
        chairs[i].room = 0;
        chairs[i].time_taken = 0;

        number_reserved--;
      }
    }
  }
}

void status_chairs() {
  printf("Status Chairs\n\n");
  for (int i = 0; i < NR; i++) {
    time_t temp_time = chairs[i].time_taken;
    if (temp_time == 0) {
      printf("Chair %-3d: Status %-2d Room %-3d Time taken --\n", chairs[i].id, chairs[i].status, chairs[i].room);
    } else {
      char temp_var[48];
      strcpy(temp_var, ctime(&chairs[i].time_taken));
      temp_var[strcspn(temp_var, "\n")] = 0;
      printf("Chair %-3d: Status %-2d Room %-3d Time taken %s\n", chairs[i].id, chairs[i].status, chairs[i].room, temp_var);
    }
  }
  printf("\n");
}

void ask_user() {
  char input;
  printf("Actions: (r) Reserve, (f) Free, (s) Status, (q) Quit\n");
  scanf(" %c", &input);
  switch (input) {
    case 'r':
      reserve_chairs();
      break;
    case 'f':
      free_chairs();
      break;
    case 's':
      status_chairs();
      break;
    case 'q':
      run = 0;
      break;
    default:
      break;
  }
}

void loop(time_t time_start) {
  while (run) {
    ask_user();
  }
}

int main (int argc, char *argv[])
{
  time_t time_start;
  time(&time_start);

  init_chairs();
  init_rooms();

  loop(time_start);

  return 0;
}

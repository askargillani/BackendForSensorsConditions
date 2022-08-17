#include"stdio.h"
#include"sensor_db.h"

static int my_special_callback(void *unused, int count, char **data, char **columns)
{
    int idx;

    printf("There are %d column(s)\n", count);

    for (idx = 0; idx < count; idx++) {
        printf("The data in column \"%s\" is: %s\n", columns[idx], data[idx]);
    }

    printf("\n");

    return 0;
}



int main()
{



	DBCONN* conn = init_connection('1');
	time_t t;
	t = time(NULL);
	/////////////////////////////////////////////////////////////////
	insert_sensor(conn, 1, 12, t);

	/////////////////////////////////////////////////////////////////
	FILE* fptr = fopen("roomSensor.txt","r");
	if(fptr)
	insert_sensor_from_file(conn, fptr);
	/////////////////////////////////////////////////////////////////
	printf("%s\n","----------------------------------------");
	find_sensor_all(conn, my_special_callback);
	
	////////////////////////////////////////////////////////////////
	printf("%s\n","----------------------------------------");
	find_sensor_by_value(conn, 49, my_special_callback);	
	
	///////////////////////////////////////////////////////////////
	printf("%s\n","----------------------------------------");
	find_sensor_by_timestamp(conn,t,my_special_callback);
	
	
	
	//////////////////////////////////////////////////////////////
	printf("%s\n","----------------------------------------");
	find_sensor_after_timestamp(conn, t, my_special_callback);
	disconnect(conn);
	
}

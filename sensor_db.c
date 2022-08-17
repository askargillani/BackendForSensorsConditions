#include "sensor_db.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>

DBCONN *init_connection(char clear_up_flag)
{
	char* log;
	sqlite3* db;
	char sql[300];
	int openStatus = sqlite3_open(TO_STRING(DB_NAME),&db);
	if(openStatus)
	{
		printf("Error: Not connecting with database!\n");
		return NULL;
	}
	else
	{
	
		
		if(clear_up_flag=='1')
		{
			memset(sql, '\0',1 );
			snprintf(sql,300,"Drop table if exists %s;",TO_STRING(TABLE_NAME));
			
			int rc = sqlite3_exec(db,sql,NULL,NULL,&log);
			if(rc!=SQLITE_OK)
			{
				printf("Error: %s\n",log);
				return NULL;
			}
		}
		
		memset(sql, '\0',1 );

		snprintf(sql,300,"CREATE TABLE %s(id int IDENTITY(1, 1) PRIMARY KEY,sensor_id int,sensor_value DECIMAL(4,2),'timest' DATETIME);",TO_STRING(TABLE_NAME));
		
		int rc = sqlite3_exec(db,sql,NULL,NULL,&log);
		if(rc!=SQLITE_OK)
		{
			printf("Error: %s\n",log);
			return NULL;
		}
		
		printf("Database connected successfully!\n");
		return db;
	
	}
}

void disconnect(DBCONN *conn)
{
	sqlite3_close(conn);
	printf("database disconnected!\n");
}

int insert_sensor(DBCONN *conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts)
{
	struct tm* ptr;
	ptr = localtime(&ts);
	
	char date[100];
	snprintf(date,100,"'%d-%d-%d %d:%d:%d'",((ptr->tm_year)+1900),ptr->tm_mon,ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);	
	
	char sql[300];
	char* log;
	snprintf(sql,300,"INSERT INTO %s (sensor_id, sensor_value, timest) VALUES(%d,%f,%s);",TO_STRING(TABLE_NAME),id,value,date);
	
	printf("Query: %s\n Results:\n",sql);
	int rc = sqlite3_exec(conn,sql,NULL,NULL,&log);
	if(rc!=SQLITE_OK)
	{
		printf("Error: %s\n",log);
		return 1;
	}
	printf("Query executed!\n");
	return 0;
	
}

int insert_sensor_from_file(DBCONN *conn, FILE *sensor_data)
{
	int num1 = 0;
	int num2 = 0;
	while(fscanf(sensor_data, "%d", &num1)!=EOF)
	{
		fscanf(sensor_data, "%d", &num2);
		time_t t;
		t = time(NULL);
		if(insert_sensor(conn,num1,num2,t)!=0)
			return 1;
	}
	printf("Query executed!\n");
	return 0;
	
}



int find_sensor_all(DBCONN *conn, callback_t f)
{
	char query[200];
	snprintf(query,200,"select* from %s",TO_STRING(TABLE_NAME));
	printf("Query: %s\n Results:\n",query);
	
	char* log;
	int rc = sqlite3_exec(conn,query,f,NULL,&log);
	
	if(rc!=SQLITE_OK)
	{
		printf("Error: %s\n",log);
		return 1;
	}
	printf("Query executed!\n");	
	return 0;
}

int find_sensor_by_value(DBCONN *conn, sensor_value_t value, callback_t f)
{
	char query[200];
	snprintf(query,200,"select* from %s where sensor_value=%f",TO_STRING(TABLE_NAME),value);
	printf("Query: %s\n Results:\n",query);
		
	char* log;
	int rc = sqlite3_exec(conn,query,f,NULL,&log);
	
	if(rc!=SQLITE_OK)
	{
		printf("Error: %s\n",log);
		return 1;
	}
	printf("Query executed!\n");
	return 0;
}

int find_sensor_exceed_value(DBCONN *conn, sensor_value_t value, callback_t f)
{
	char query[200];
	snprintf(query,200,"select* from %s where sensor_value>%f",TO_STRING(TABLE_NAME),value);
	
	printf("Query: %s\n Results:\n",query);
	char* log;
	int rc = sqlite3_exec(conn,query,f,NULL,&log);
	
	if(rc!=SQLITE_OK)
	{
		printf("Error: %s\n",log);
		return 1;
	}
	printf("Query executed!\n");
	return 0;
}

int find_sensor_by_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f)
{
	struct tm* ptr;
	ptr = localtime(&ts);
	
	char date[100];
	snprintf(date,100,"'%d-%d-%d %d:%d:%d'",((ptr->tm_year)+1900),ptr->tm_mon,ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);	

	char query[400];
	snprintf(query,400,"select* from %s where timest= %s",TO_STRING(TABLE_NAME),date);
	printf("Query: %s\n Results:\n",query);
	
	char* log;
	int rc = sqlite3_exec(conn,query,f,NULL,&log);
	
	if(rc!=SQLITE_OK)
	{
		printf("Error: %s\n",log);
		return 1;
	}
	printf("Query executed!\n");
	return 0;
}

int find_sensor_after_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f)
{
	struct tm* ptr;
	ptr = localtime(&ts);
	
	char date[100];
	snprintf(date,100,"'%d-%d-%d %d:%d:%d'",((ptr->tm_year)+1900),ptr->tm_mon,ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);	

	char query[200];
	snprintf(query,200,"select* from %s where timest>= %s",TO_STRING(TABLE_NAME),date);
	printf("Query: %s\n Results:\n",query);
	char* log;
	int rc = sqlite3_exec(conn,query,f,NULL,&log);
	
	if(rc!=SQLITE_OK)
	{
		printf("Error: %s\n",log);
		return 1;
	}
	printf("Query executed!\n");
	return 0;
}




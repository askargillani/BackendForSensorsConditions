#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "sqlite3.h"

// stringify preprocessor directives using 2-level preprocessor magic
// this avoids using directives like -DDB_NAME=\"some_db_name\"
#define REAL_TO_STRING(s) #s
#define TO_STRING(s) REAL_TO_STRING(s)    //force macro-expansion on s before stringify s

#ifndef DB_NAME
#define DB_NAME Sensor.db
#endif

#ifndef TABLE_NAME
#define TABLE_NAME SensorData
#endif

#define DBCONN sqlite3

typedef int (*callback_t)(void *, int, char **, char **);

/**
 * Make a connection to the database server
 * Create (open) a database with name DB_NAME having 1 table named TABLE_NAME  
 * \param clear_up_flag if the table existed, clear up the existing data when clear_up_flag is set to 1
 * \return the connection for success, NULL if an error occurs
 */
DBCONN *init_connection(char clear_up_flag);
void disconnect(DBCONN *conn);
int insert_sensor(DBCONN *conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts);
int insert_sensor_from_file(DBCONN *conn, FILE *sensor_data);
int find_sensor_all(DBCONN *conn, callback_t f);
int find_sensor_by_value(DBCONN *conn, sensor_value_t value, callback_t f);
int find_sensor_exceed_value(DBCONN *conn, sensor_value_t value, callback_t f);
int find_sensor_by_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f);
int find_sensor_after_timestamp(DBCONN *conn, sensor_ts_t ts, callback_t f);

#endif /* _SENSOR_DB_H_ */

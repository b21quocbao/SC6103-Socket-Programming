#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "serialization.h"

#define SUCCESS 1
#define ERROR 0

// Flight database (for simplicity, we are using an array)
#define MAX_FLIGHTS 100
Flight flight_db[MAX_FLIGHTS];
int flight_count = 0;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Utility function to generate random departure time (Unix timestamp)
uint32_t random_departure_time()
{
    time_t now = time(NULL);
    // Generate a random departure time within the next 30 days
    return now + random_int(0, 30 * 24 * 3600); // 30 days in seconds
}

// Function to seed flight data
void seed_flight_data()
{
    const char *cities[] = {
        "Beijing", "Shanghai", "Guangzhou", "Shenzhen", "Chengdu",
        "Hangzhou", "Chongqing", "Nanning", "Kunming", "Wuhan",
        "Bangkok", "Singapore", "Jakarta", "Kuala Lumpur", "Manila",
        "Hanoi", "Ho Chi Minh City", "Phnom Penh", "Vientiane", "Yangon"};

    // Seed the random number generator
    srand(time(NULL));

    // Seed the flight data
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            flight_db[j * 20 + i].id = j * 20 + i; // Unique flight IDs starting from 1

            // Allocate memory for source and destination cities
            flight_db[j * 20 + i].src = malloc(50 * sizeof(char));
            flight_db[j * 20 + i].dest = malloc(50 * sizeof(char));

            // Randomly select a source and destination from the cities array
            strcpy(flight_db[j * 20 + i].src, cities[i % 20]);
            strcpy(flight_db[j * 20 + i].dest, cities[(i + 1) % 20]);

            // Generate a random departure time (within the next 30 days)
            flight_db[j * 20 + i].dep = random_departure_time();

            // Generate a random airfare between $100 and $1000
            flight_db[j * 20 + i].fare = random_fare(100.0, 1000.0);

            // Generate a random number of available seats between 50 and 200
            flight_db[j * 20 + i].seat_avail = random_int(50, 200);
        }
    }

    flight_count = 60;
}

// Function to print flight data
void print_flight_data()
{
    printf("Flight Data:\n");
    printf("---------------------------------------------------------------\n");
    printf("| ID   | Source               | Destination          | Departure Time | Fare    | Seats |\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < flight_count; i++)
    {
        printf("| %4d | %-20s | %-20s | %13u | $%-6.2f | %5d |\n",
               flight_db[i].id, flight_db[i].src, flight_db[i].dest, flight_db[i].dep, flight_db[i].fare, flight_db[i].seat_avail);
    }

    printf("---------------------------------------------------------------\n");
}

void query_flights_by_src_dest(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len)
{
    uint32_t src_len, dest_len;
    char src[100], dest[100];
    size_t offset = 3;

    // Extract source and destination from input
    memcpy(&src_len, input + offset, sizeof(src_len));
    src_len = ntohl(src_len);
    offset += sizeof(src_len);
    printf("Extracted: src_len=%d\n", src_len);

    memcpy(src, input + offset, src_len);
    offset += src_len;
    printf("Extracted: src=%s\n", src);

    memcpy(&dest_len, input + offset, sizeof(dest_len));
    dest_len = ntohl(dest_len);
    offset += sizeof(dest_len);
    printf("Extracted: dest_len=%d\n", dest_len);

    memcpy(dest, input + offset, dest_len);
    offset += dest_len;
    printf("Extracted: dest=%s\n", dest);

    int num_flights = 0;

    // Check each flight
    for (int i = 0; i < flight_count; i++)
    {
        if (strncmp(flight_db[i].src, src, src_len) == 0 && strncmp(flight_db[i].dest, dest, dest_len) == 0)
        {
            num_flights++;
        }
    }

    if (num_flights > 0)
    {
        prepend_msg(output, SUCCESS, "Flights found", output_len);
    }
    else
    {
        prepend_msg(output, ERROR, "No flights matched destination and source", output_len);
    }

    // Check each flight
    for (int i = 0; i < flight_count; i++)
    {
        if (strncmp(flight_db[i].src, src, src_len) == 0 && strncmp(flight_db[i].dest, dest, dest_len) == 0)
        {
            serialize_flight(&flight_db[i], output, output_len);
        }
    }
}

void query_flight_by_id(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len)
{
    uint32_t flight_id;
    size_t offset = 3;

    // Extract flight id from input
    memcpy(&flight_id, input + offset, sizeof(flight_id));
    flight_id = ntohl(flight_id);

    for (int i = 0; i < flight_count; i++)
    {
        if (flight_db[i].id == flight_id)
        {
            prepend_msg(output, SUCCESS, "Flight found", output_len);
            serialize_flight(&flight_db[i], output, output_len);
            return;
        }
    }

    prepend_msg(output, ERROR, "Flight not found", output_len);
}

void reserve_seats(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len)
{
    uint32_t flight_id, num_seats;
    size_t offset = 3;

    // Extract flight id and number of seats from input
    memcpy(&flight_id, input + offset, sizeof(flight_id));
    flight_id = ntohl(flight_id);
    offset += sizeof(flight_id);

    memcpy(&num_seats, input + offset, sizeof(num_seats));
    num_seats = ntohl(num_seats);
    offset += sizeof(num_seats);

    for (int i = 0; i < flight_count; i++)
    {
        if (flight_db[i].id == flight_id)
        {
            if (flight_db[i].seat_avail >= num_seats)
            {
                flight_db[i].seat_avail -= num_seats;
                prepend_msg(output, SUCCESS, "Reserved successfully", output_len);
                serialize_flight(&flight_db[i], output, output_len);
            }
            else
            {
                prepend_msg(output, ERROR, "Seats available below your demands", output_len);
            }
            return;
        }
    }

    prepend_msg(output, ERROR, "Flight not found", output_len);
}

// Simulating a client registering for seat updates
void register_for_seat_updates(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len, uint32_t monitor_duration)
{
    uint32_t flight_id;
    size_t offset = 3;

    // Extract flight id from input
    memcpy(&flight_id, input + offset, sizeof(flight_id));
    flight_id = ntohl(flight_id);
    offset += sizeof(flight_id);

    // Simulate monitoring for seat availability updates (this would be an ongoing process)
    for (int i = 0; i < flight_count; i++)
    {
        if (flight_db[i].id == flight_id)
        {
            serialize_flight(&flight_db[i], output, output_len);
            *output_len += 1;

            // Simulate waiting for updates (e.g., for monitor_duration ms)
            // In a real-world app, this would block the client from sending new requests until monitoring ends.
            printf("Monitoring seat availability for flight %d...\n", flight_id);
            // You can simulate the waiting period here.
            return;
        }
    }

    prepend_msg(output, ERROR, "Flight not found", output_len);
}

void query_flights_by_src_fare_range(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len)
{
    uint32_t src_len;
    double min_fare, max_fare;
    char src[100];
    size_t offset = 3;

    // Extract source length, source string, min fare, and max fare from input
    memcpy(&src_len, input + offset, sizeof(src_len));
    src_len = ntohl(src_len);
    offset += sizeof(src_len);

    memcpy(src, input + offset, src_len);
    offset += src_len;

    memcpy(&min_fare, input + offset, sizeof(min_fare));
    min_fare = ntohl(min_fare);
    offset += sizeof(min_fare);

    memcpy(&max_fare, input + offset, sizeof(max_fare));
    max_fare = ntohl(max_fare);
    offset += sizeof(max_fare);

    int num_flights = 0;

    // Check each flight
    for (int i = 0; i < flight_count; i++)
    {
        if (strncmp(flight_db[i].src, src, src_len) == 0 && flight_db[i].fare >= min_fare && flight_db[i].fare <= max_fare)
        {
            num_flights++;
        }
    }

    if (num_flights > 0)
    {
        prepend_msg(output, SUCCESS, "Flights found", output_len);
    }
    else
    {
        prepend_msg(output, ERROR, "No flights matched source and fare", output_len);
    }

    // Check each flight
    for (int i = 0; i < flight_count; i++)
    {
        if (strncmp(flight_db[i].src, src, src_len) == 0 && flight_db[i].fare >= min_fare && flight_db[i].fare <= max_fare)
        {
            serialize_flight(&flight_db[i], output, output_len);
        }
    }
}

void delay_flight(uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len)
{
    uint32_t flight_id, delay_ms;
    size_t offset = 3;

    // Extract flight id and delay time from input
    memcpy(&flight_id, input + offset, sizeof(flight_id));
    flight_id = ntohl(flight_id);
    offset += sizeof(flight_id);

    memcpy(&delay_ms, input + offset, sizeof(delay_ms));
    delay_ms = ntohl(delay_ms);
    offset += sizeof(delay_ms);

    for (int i = 0; i < flight_count; i++)
    {
        if (flight_db[i].id == flight_id)
        {
            flight_db[i].dep += delay_ms; // Add delay to departure time
            
            prepend_msg(output, ERROR, "Flight delayed successfully", output_len);
            serialize_flight(&flight_db[i], output, output_len);

            return;
        }
    }

    prepend_msg(output, ERROR, "Flight not found", output_len);
}
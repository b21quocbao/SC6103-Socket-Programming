# Socket Programming Project

This project demonstrates a simple client-server application using socket programming in C++.

## Prerequisites

Ensure that you have `g++`, `gcc` and `makke` installed on your system. You can install them using the following commands:

### For Ubuntu/WSL:
```bash
sudo apt update
sudo apt install g++ gcc make
```

## Setup and Usage

### 1. Server Setup
To set up the server, follow these steps:

1. Compile and run the server:
   ```bash
   make all && ./flight_reservation_system
   ```

## Diagram

```mermaid
sequenceDiagram
    participant User as Client
    participant Server as Server

    User->>Server: 1. Query flights by source and destination
    Note over User,Server: Request Format: [src len][src][dest len][dest]
    alt Multiple flights found
        Server-->>User: Response: [1][msg len][Success Msg][num flights][id][src len][src][dest len][dest][dep][fare][seat avail] (for each flight)
    else No flights found
        Server-->>User: Response: [0][msg len][Error Msg]
    end

    User->>Server: 2. Query flight details by id
    Note over User,Server: Request Format: [id]
    alt Flight exists
        Server-->>User: Response: [1][msg len][Success Msg][id][src len][src][dest len][dest][dep][fare][seat avail]
    else Flight does not exist
        Server-->>User: Response: [0][msg len][Error Msg]
    end

    User->>Server: 3. Reserve seats by id and number of seats
    Note over User,Server: Request Format: [id][num seats]
    alt Reservation successful
        Server-->>User: Response: [1][msg len][Success Msg][id][src len][src][dest len][dest][dep][fare][updated seat avail]
    else Incorrect input or insufficient seats
        Server-->>User: Response: [0][msg len][Error Msg]
    end

    User->>Server: 4. Register for seat avail updates
    Note over User,Server: Request Format: [id][monitor interval]
    Server->>Server: Record client address, port, flight ID, monitor interval
    loop During monitor interval
        alt Seat reservation made
            Server-->>User: Response: [1][msg len][Updated Msg][id][src len][src][dest len][dest][dep][fare][updated seat avail]
        end
    end
    Server->>Server: Remove client record after monitor interval expires
    Server-->>User: Response: [0][msg len][Monitor Interval Expired Msg]

    User->>Server: 5. Query flights by source and airfare range
    Note over User,Server: Request Format: [src len][src][min fare][max fare]
    alt Multiple flights found within range
        Server-->>User: Response: [1][msg len][Success Msg][num flights][id][src len][src][dest len][dest][dep][fare][seat avail] (for each flight)
    else No flights found within range
        Server-->>User: Response: [0][msg len][Error Msg]
    end

    User->>Server: 6. Delay flight by id and duration in Ms
    Note over User,Server: Request Format: [id][delay in Ms]
    alt Flight exists and delay applied
        Server-->>User: Response: [1][msg len][Success Msg][id][src len][src][dest len][dest][new dep][fare][seat avail]
    else Flight does not exist
        Server-->>User: Response: [0][msg len][Error Msg]
    end

```
### Prepend message
- Each request/reply needs to prepend `[message_type][request_id][service_type]`

### Shortened Variable Names Explanation

1. **identifier** → `id`
2. **source** → `src`
3. **destination** → `dest`
4. **departure time in Ms** → `dep`
5. **airfare** → `fare`
6. **seat availability** → `seat avail`
7. **number of seats** → `num seats`
8. **number of flights** → `num flights`
9. **Success Message** → `Success Msg`
10. **Error Message** → `Error Msg`
11. **Updated Message** → `Updated Msg`

These abbreviations are used in the mermaid diagram to make the request and response format more concise and readable.

### Note for Request/Response Format:

- **0/1 (Success/Error)**: 1 byte (8 bits)
- **String length fields (e.g., msg len, src len, dest len)**: 32 bits (4 bytes) for each string length
- **fare**: Double-precision floating-point value (64 bits, 8 bytes)
- **id, seat avail, dep (departure time in Ms)**: 32-bit integers (4 bytes each)

This note ensures clarity for the size and format of all string length fields and other key values in the request and response structure.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

Thanks to the open-source community for providing excellent resources and documentation on socket programming in C++.

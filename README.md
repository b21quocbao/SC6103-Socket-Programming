# Socket Programming Project

This project demonstrates a simple client-server application using socket programming in C++.

## Prerequisites

Ensure that you have `g++` and `gcc` installed on your system. You can install them using the following commands:

### For Ubuntu/WSL:
```bash
sudo apt update
sudo apt install g++ gcc
```

## Setup and Usage

### 1. Server Setup
To set up the server, follow these steps:

1. Compile and run the server:
   ```bash
   g++ -o build/server server.cpp && ./build/server
   ```

### 2. Client Setup
To set up the client, follow these steps:

1. Compile and run the client:
   ```bash
   g++ -o build/client client.cpp && ./build/client
   ```

2. Enter the server host:
   ```bash
   Enter server host: localhost
   ```

3. Enter the server port:
   ```bash
   Enter server port: 3000
   ```

4. Enter your name:
   ```bash
   Enter your name: John
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

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

Thanks to the open-source community for providing excellent resources and documentation on socket programming in C++.

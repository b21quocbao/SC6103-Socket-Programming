import java.util.Date;

public class FlightData {
    private int id;
    private String src;
    private String dest;
    private Date depTime;
    private double fare;
    private int seats;

    public FlightData(int id, String src, String dest, Date depTime, double fare, int seats) {
        this.id = id;
        this.src = src;
        this.dest = dest;
        this.depTime = depTime;
        this.fare = fare;
        this.seats = seats;
    }

    @Override
    public String toString() {
        return "[Flight ID: " + id + ", Source: " + src + ", Destination: " + dest +
                ", Departure Time: " + depTime + ", Fare: " + fare + ", Number of Available Seats: " + seats +"]\n";
    }
}

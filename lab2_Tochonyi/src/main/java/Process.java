public class Process implements Comparable<Process> {
  public int cpu_time;
  public int cpu_done = 0;
  public int arrival_time;

  public int deadline;

  public int id;
  public int all_time;

  public Process(int id, int cpu_time, int arrival_time, int deadline) {
    this.id = id;
    this.cpu_time = cpu_time;
    this.arrival_time = arrival_time;
    this.deadline = deadline;
  }

  public int remainingTime(int currentTime){
    return deadline - currentTime;
  }

  public boolean done(){
    return cpu_time == cpu_done;
  }

  @Override
  public int compareTo(Process other) {
    return Integer.compare(this.deadline, other.deadline);
  }
}

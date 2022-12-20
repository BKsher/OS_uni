import java.util.ArrayList;
import java.io.*;
import java.util.Collections;

public class SchedulingAlgorithm {
  private static int compTime = 0;

  public static void Run(ArrayList<Process> processVector, Results result) {
    int completed = 0;
    Process process;
    String resultsFile = "Summary-Processes";

    result.schedulingType = "Batch";
    result.schedulingName = "Earliest Deadline First";

    try {
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));

      while (completed < processVector.size()) {
        process = getNextProcess(processVector);
        if (process == null) {
          // All processes have completed or the list is empty
          break;
        }
        process.all_time = compTime;
        process.cpu_done++;
        process.deadline--;
        compTime++;
        out.println("Process: " + process.id + " executed at time " + compTime + " (deadline = " + process.deadline + ")");
        if (process.done()) {
          completed++;
          out.println("Process: " + process.id + " completed at time " + compTime);
        }
      }
      out.close();
    } catch (IOException e) {
      System.out.println(e.getMessage());
    }
    result.compuTime = compTime;
  }

  public static Process getNextProcess(ArrayList<Process> processVector) {
    // Sort the list of processes in ascending order of deadline
    Collections.sort(processVector);

    // Iterate through the list and return the first process that has not completed and has a positive deadline
    for (Process p : processVector) {
      if (!p.done() && p.deadline > 0) {
        return p;
      }
    }
    // Return null if no eligible process was found
    return null;
  }
}

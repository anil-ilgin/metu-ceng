import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.Array;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Scanner;

public class CengTreeParser
{
    public static ArrayList<CengVideo> parseVideosFromFile(String filename)
    {
        ArrayList<CengVideo> videoList = new ArrayList<CengVideo>();

        // You need to parse the input file in order to use GUI tables.
        //TODO: Parse the input file, and convert them into CengVideos
        try {
            File file = new File(filename);
            Scanner scan = new Scanner(file);
            String [] list;
            CengVideo added;
            int key;
            while(scan.hasNextLine()) {
                String data = scan.nextLine();
                list = data.split("\\|",4);
                key = Integer.parseInt(list[0]);
                added = new CengVideo(key,list[1],list[2],list[3]);
                videoList.add(added);
            }
            scan.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("An error has occured");
            e.printStackTrace();
        }
        return videoList;
    }

    public static void startParsingCommandLine() throws IOException
    {
        // TODO: Start listening and parsing command line -System.in-.
        // There are 4 commands:
        // 1) quit : End the app, gracefully. Print nothing, call nothing, just break off your command line loop.
        // 2) add : Parse and create the video, and call CengVideoRunner.addVideo(newlyCreatedVideo).
        // 3) search : Parse the key, and call CengVideoRunner.searchVideo(parsedKey).
        // 4) print : Print the whole tree, call CengVideoRunner.printTree().

        // Commands (quit, add, search, print) are case-insensitive.
        String quit = "quit";
        String add = "add";
        String search = "search";
        String print = "print";
        String [] list;
        int key;
        Scanner scan = new Scanner(System.in);
        boolean is_active = true;
        while (is_active) {
            String command = scan.nextLine();
            list = command.split("\\|",5);
            list[0] = list[0].toLowerCase(Locale.ROOT);

            if (list[0].equals(quit)) { is_active = false; }
            else if (list[0].equals(add)) {
                key = Integer.parseInt(list[1]);
                CengVideo added = new CengVideo(key,list[2],list[3],list[4]);
                CengVideoRunner.addVideo(added);
            }
            else if (list[0].equals(search)) {
                key = Integer.parseInt(list[1]);
                CengVideoRunner.searchVideo(key);
            }
            else if (list[0].equals(print)) {
                CengVideoRunner.printTree();
            }
            else {
                System.out.println("Invalid command, please enter a valid one");
            }
        }

    }
}

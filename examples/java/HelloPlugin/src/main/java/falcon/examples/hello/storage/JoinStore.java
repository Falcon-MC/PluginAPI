package falcon.examples.hello.storage;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;

public final class JoinStore {
    private final Map<String, Integer> mJoins = new ConcurrentHashMap<>();
    private Path mPath;

    public void load(Path path) {
        mPath = path;
        mJoins.clear();
        if (!Files.isRegularFile(path)) {
            return;
        }

        try (Scanner scanner = new Scanner(path, StandardCharsets.UTF_8)) {
            while (scanner.hasNext()) {
                String name = scanner.next();
                if (!scanner.hasNextInt()) {
                    break;
                }
                mJoins.put(name, scanner.nextInt());
            }
        } catch (IOException exception) {
            mJoins.clear();
        }
    }

    public void save() {
        if (mPath == null) {
            return;
        }

        try (BufferedWriter writer = Files.newBufferedWriter(mPath, StandardCharsets.UTF_8)) {
            for (Map.Entry<String, Integer> entry : mJoins.entrySet()) {
                writer.write(entry.getKey() + " " + entry.getValue() + "\n");
            }
        } catch (IOException exception) {
            throw new IllegalStateException("Could not save " + mPath, exception);
        }
    }

    public int recordJoin(String playerName) {
        return mJoins.merge(playerName, 1, Integer::sum);
    }

    public int joinsOf(String playerName) {
        return mJoins.getOrDefault(playerName, 0);
    }
}

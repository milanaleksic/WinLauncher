package net.milanaleksic.winlauncher;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.channels.FileLock;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class WinLauncherUtil {

    private static final String CONFIGURATION_FILENAME = ".launcher";

    private static final String WIN_LAUNCHER_ENV_PARAM_WAIT_ON_STARTUP = "winLauncherEnvParamWaitOnStartup";

    private static final String WIN_LAUNCHER_EXECUTABLE = "WinLauncher.exe";

    public static final int WIN_LAUNCHER_DEFAULT_PAUSE_TIME = 5000;

    public static FileLock createSingletonApplicationFileLock() {
        try {
            File configurationFile = new File(CONFIGURATION_FILENAME);
            if (!configurationFile.exists())
                throw new WinLauncherException("Locking failure - no configuration file found");
            FileLock lock = new RandomAccessFile(configurationFile, "rw").getChannel().tryLock();
            if (lock == null)
                throw new WinLauncherException("Locking failure - probably another application is already holding the lock");
            return lock;
        } catch (WinLauncherException ignored) {
            throw ignored;
        } catch (Exception e) {
            throw new WinLauncherException("Locking failure - " + e.getMessage(), e);
        }
    }

    public static void closeSingletonApplicationFileLock(FileLock fileLock) {
        try {
            if (fileLock != null)
                fileLock.channel().close();
        } catch (IOException e) {
            throw new WinLauncherException("Unlocking failure - " + e.getMessage(), e);
        }
    }

    public static void wrapSingletonApplicationLogic(ApplicationLogic applicationLogic) {
        FileLock lock = null;
        try {
            lock = createSingletonApplicationFileLock();
            applicationLogic.setFileLock(lock);
            applicationLogic.run();
        } catch (Exception e) {
            applicationLogic.couldNotRun(e);
        } finally {
            closeSingletonApplicationFileLock(lock);
        }
    }

    private static String getParentDir() {
        String absPath = new File("").getAbsolutePath();
        if (absPath.charAt(absPath.length() - 1) == File.separatorChar)
            absPath = absPath.substring(0, absPath.length() - 1);
        return absPath.substring(0, absPath.lastIndexOf(File.separatorChar)) + File.separatorChar;
    }

    public static void restartApplication() {
        restartApplication(WIN_LAUNCHER_DEFAULT_PAUSE_TIME);
    }

    public static void restartApplication(int pauseTimeMillis) {
        try {
            String dir;
            if (new File(WIN_LAUNCHER_EXECUTABLE).exists())
                dir = new File("").getAbsolutePath() + File.separator;
            else
                dir = getParentDir();

            Map<String, String> env = System.getenv();
            List<String> list = new ArrayList<String>();
            for (Map.Entry<String, String> entry : env.entrySet()) {
                list.add(entry.getKey() + "=" + entry.getValue());
            }

            list.add(WIN_LAUNCHER_ENV_PARAM_WAIT_ON_STARTUP + "=" + pauseTimeMillis);

            String[] arr = new String[list.size()];
            for (int i = 0; i < list.size(); i++) {
                arr[i] = list.get(i);
            }

            Runtime.getRuntime().exec(dir + WIN_LAUNCHER_EXECUTABLE, arr, new File(dir));

        } catch (Exception e) {
            throw new WinLauncherException("Problem while trying to restart application - "+e.getMessage(), e);
        }
    }
}

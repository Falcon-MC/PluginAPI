package falcon.api;

/** Map ids row by row from the top left; empty when the grid could not be made. */
public record ImageMapGrid(int columns, int rows, long[] ids) {
    public boolean isEmpty() {
        return ids.length == 0;
    }
}

import javax.swing.*;
import java.util.*;
import javax.swing.table.*;

public class Model extends AbstractTableModel{

	Vector rowData, columnNames;

    AlbumCollection acl;
    Playlist plt;
	
	public Model(AlbumCollection ac, Playlist pt) {

        acl = ac;
        plt = pt;

		columnNames = new Vector();
		columnNames.add("Album");
		columnNames.add("Track");
		columnNames.add("Duration");

		rowData = new Vector();
    }

    public void updateAlbumlist() {
        rowData.clear();
        ArrayList<String[]> list = PlayerFactory.getAlbumsTrack(acl);

        for (int i = 0; i < list.size(); i++) {
            Vector row = new Vector();

            row.add(list.get(i)[0]);
            row.add(list.get(i)[1]);
            row.add(list.get(i)[2]);

            rowData.add(row);
        }
    }

    public void updatePlaylist() {
        rowData.clear();
        ArrayList<String[]> list = PlayerFactory.getPlaylistTrack(plt);

        for (int i = 0; i < list.size(); i++) {
            Vector row = new Vector();

            row.add(list.get(i)[0]);
            row.add(list.get(i)[1]);
            row.add(list.get(i)[2]);

            rowData.add(row);
        }
    }

	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return this.columnNames.size();
	}

	@Override
	public int getRowCount() {
		// TODO Auto-generated method stub
		return this.rowData.size();
	}

	@Override
	public Object getValueAt(int rowIndex, int column) {
		// TODO Auto-generated method stub
		return ((Vector)this.rowData.get(rowIndex)).get(column);
	}

	@Override
	public String getColumnName(int arg0) {
		// TODO Auto-generated method stub
		return (String) this.columnNames.get(arg0);
	}
}

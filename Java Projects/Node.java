/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tree23;

/**
 *
 * @author Elio
 */

public class Node {
    private final int ORDER = 3;
    private int numItems;
    private Node parent;
    private Node childArray[] = new Node[ORDER];
    private DataItem[] dataArray = new DataItem[ORDER-1];

    public void connectChild(int childNum, Node child) {
        childArray[childNum] = child;
        if (child != null) {
            child.parent = this;
        }
    }

    public Node disconnectChild(int childNum) {
        Node t = childArray[childNum];
        childArray[childNum] = null;
        return t;
    }

    public Node getChild(int childNum) {
        return childArray[childNum];
    }

    public Node getParent() {
        return parent;
    }

    public boolean isLeaf() {
        return childArray[0] == null;
    }

    public int getNumItems() {
        return numItems;
    }

    public boolean isFull() {
        return numItems == ORDER - 1;
    }

    public DataItem getItem(int index) {
        return dataArray[index];
    }

    //returns the index of dataArray, or -1 if not found
    public int findItem(long data) {
        for (int i=0; i<numItems; i++)
            if (data == dataArray[i].data)
                return i;
        
        return -1;
    }

    public int insertItem(DataItem dataItem) {
        numItems++;
        for (int i=ORDER-2; i >= 0; i--) {  //Rearrange data items
            if (dataArray[i] == null)
                continue;
            else {
                if(dataArray[i].data > dataItem.data)
                    dataArray[i+1] = dataArray[i];
                else {
                    dataArray[i+1] = dataItem;
                    return i+1;
                }
            }            
        }
        
        dataArray[0] = dataItem;    //For emtpy node
        return 0;
    }

    public DataItem removeRightMostItem() {
        DataItem t = dataArray[numItems - 1];
        dataArray[numItems - 1] = null;
        numItems--;
        
        return t;
    }

    public void displayNode() {
        for (int i = 0; i < numItems; i++)
            System.out.print("/" + dataArray[i].data);
        
        System.out.println("/");
    }
}
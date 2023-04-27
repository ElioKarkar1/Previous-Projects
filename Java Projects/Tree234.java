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
public class Tree234 {
    private Node root = new Node();

    public int find(long key) {
        Node current = root;
        int childIndex;
        
        while (true) {
            childIndex = current.findItem(key);
            if (childIndex != -1) 
                return childIndex;
            else if (current.isLeaf())
                return -1;
            else
                current = getNextChild(current, key);
        }       
    }
    
    private Node getNextChild(Node current, long value) {
        int i;
        
        for (i=0; i<current.getNumItems(); i++) {
            if (value <current.getItem(i).data)
                return current.getChild(i);
        }
        
        return current.getChild(i);
    }
    
    public void insert(long value) {
        DataItem dataItem = new DataItem(value);
        Node current = root;
        
        while (true) {           
            if (current.isFull()) {
                split(current);
                current = current.getParent();  //Go back one level to figure out which child should be visited next
            } else if (current.isLeaf()) {
                break;
            }
            
            current = getNextChild(current, value);            
        }
        
        current.insertItem(dataItem);
    }
    
    private void split(Node node) {
        DataItem itemB, itemC;
        Node parent, child2, child3;
        int itemIndex;
        
        itemC = node.removeRightMostItem();
        itemB = node.removeRightMostItem();
        child2 = node.disconnectChild(1);
        child3 = node.disconnectChild(2);
        
        if (node == root) {
            root = new Node();
            parent = root;
            root.connectChild(0, node);
        } else {
            parent = node.getParent();
        }
        
        itemIndex = parent.insertItem(itemB);
        int n = parent.getNumItems();
        for (int i=n-1; i>itemIndex; i--) {     //Rearrange children if dataItems were rearranged
            Node t = parent.disconnectChild(i);
            parent.connectChild(i+1, t);
        }
        
        Node newRight = new Node();
        newRight.insertItem(itemC);
        newRight.connectChild(0, child2);
        newRight.connectChild(1, child3);
        parent.connectChild(itemIndex+1, newRight);
        
    }
    
    public void display() {
        recursiveTraverse(root);
    }
    
    private void recursiveTraverse(Node node) {
        if (node == null)
            return;
        
        node.displayNode();
        for (int i=0; i<3; i++) {
            recursiveTraverse(node.getChild(i));
        }
    }

}
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Graph;

import java.util.*;

/**
 *
 * @author Elio
 */
public class Graph {

    private final boolean[] visited;
    private final int vertices;
    private final LinkList[] adjacency;

    public Graph(int vertices) {
        this.vertices = vertices;
        this.adjacency = new LinkList[vertices];
        this.visited = new boolean[vertices];
        for (int i = 0; i < vertices; ++i) {
            adjacency[i] = new LinkList();
        }
        for (int i = 0; i < vertices; ++i) {
            visited[i] = false;
        }
    }

    public int find() {
        for (int i = 0; i < vertices; ++i) {
            if (visited[i] == false) {
                return i;
            }
        }
        return -1;
    }

    public void addEdge(int source, int destination) {
        this.adjacency[source].insertFirst(destination, 2 * destination);
        this.adjacency[destination].insertFirst(source, 2 * source);
    }

    public void dfs(int startingVertex) {
        visited[startingVertex] = true;
        System.out.print(startingVertex + " ");
        Link head = adjacency[startingVertex].first;
        while (head != null) {
            if (visited[head.key] == false) {
                dfs(head.key);
            }
            head = head.next;
        }
    }

    public static void main(String[] args) {

        Graph g = new Graph(10);
        g.addEdge(5, 7);
        g.addEdge(6, 8);
        g.dfs(1);
        System.out.println("\n" + g.find() + " has not been visited");
        g.dfs(0);
        System.out.println("\n" + g.find() + " has not been visited");
    }
}

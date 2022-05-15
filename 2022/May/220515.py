# DKU Algorithm Lecture Assignment
INF = int(1e9)

def dijkstra(graph):
    graph_len = len(graph)
    distance = [INF for _ in range(graph_len)]
    visited = [False for _ in range(graph_len)]
    distance[0] = 0
    visited[0] = True

    for i in range(graph_len-1):
        min_cost = INF
        min_idx = 0
        for j in range(graph_len):
            if (not visited[j]) and (min_cost > distance[j]):
                min_idx = j
                min_cost = distance[j]

        for k in range(graph_len):
            if (not visited[k]) and (graph[min_idx][k]) and (distance[min_idx] != INF) and (distance[k] > distance[min_idx] + graph[min_idx][k]):
                distance[k] = distance[min_idx] + graph[min_idx][k]
        
        visited[min_idx] = True

    print(distance)


if __name__ == '__main__':
    graph = [
        [0, 5, 0, 2, 4, 0],
        [5, 0, 3, 3, 0, 0],
        [0, 3, 0, 3, 4, 2],
        [2, 3, 3, 0, 1, 0],
        [4, 0, 4, 1, 0, 2],
        [0, 0, 2, 0, 2, 0]
    ]
    graph1 = [
        [0, 7, 4, 6, 1],
        [0, 0, 0, 0, 0],
        [0, 2, 0, 5, 0],
        [0, 3, 0, 0, 0],
        [0, 0, 0, 1, 0]
    ]
    graph2 = [
        [0, 4, 0, 0, 0, 0, 0, 8, 0],
        [4, 0, 8, 0, 0, 0, 0, 11, 0],
        [0, 8, 0, 7, 0, 4, 0, 0, 2],
        [0, 0, 7, 0, 9, 14, 0, 0, 0],
        [0, 0, 0, 9, 0, 10, 0, 0, 0],
        [0, 0, 4, 14, 10, 0, 2, 0, 0],
        [0, 0, 0, 0, 0, 2, 0, 1, 6],
        [8, 11, 0, 0, 0, 0, 1, 0, 7],
        [0, 0, 2, 0, 0, 0, 6, 7, 0]
    ]

    dijkstra(graph)     # test-case 1
    dijkstra(graph1)    # test-case 2
    dijkstra(graph2)    # test-case 3
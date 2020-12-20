#include <iostream>
#include <vector>
#include <cstdio>
#include <queue>
#include <bits/stdc++.h>

struct Edge
{
    int x, y, waga, koszt;
};

int min_cost(const std::vector<Edge>& edges, int numberOfFlowUnits, int s, int t, int costMax);

int main() {
    int numberOfSets;

    std::scanf("%d",&numberOfSets);

    while(numberOfSets > 0){

        int result=0;
        int costMax;
        std::scanf("%d",&costMax);
        int numberOfPlayers;
        std::scanf("%d",&numberOfPlayers);
        int costMin;
        int lowerLimit=int(numberOfPlayers / 2);
        int numberOfMatch = ((numberOfPlayers * (numberOfPlayers - 1)) / 2);
        std::vector<Edge> edges;
        int win[numberOfPlayers];
        int players[numberOfPlayers];
        players[0]=INT32_MAX;
        int costCounter=0;
        for(int i=0; i < numberOfPlayers; i++)
        {
            win[i]=0;
        }

        for (int i = 0; i < numberOfMatch; i++)
        {
            int x, y, w, c;
            std::scanf("%d %d %d %d", &x, &y, &w, &c);

            win[w]= win[w] + 1;

            if(x==0)
            {
                players[y] = c;
            }

            if(y==0)
            {
                players[x]=c;

            }
            if(w==x)
            {
                edges.push_back({x, y, 1, c});
            }
            else {
                edges.push_back({y, x, 1, c});
            }

        }
        std::sort(players, players + numberOfPlayers);
        int kingMayWin=win[0];
        for(int i=win[0]; i < numberOfPlayers; i++)

        {

            if(costCounter + players[i] <= costMax)
            {
                costCounter= costCounter + players[i];
                kingMayWin+=1;
            }

        }
        for(int i = 0; i < numberOfPlayers; i++)
        {
            edges.push_back({numberOfPlayers, i, win[i], 0});

        }
        if(win[0] > lowerLimit)
        {
            lowerLimit=win[0];
        }
        int upperLimit= numberOfPlayers - 1;

        if(win[0] + kingMayWin < numberOfPlayers){

            upperLimit= win[0] + kingMayWin;
        }

        for(int j=lowerLimit; j <= upperLimit and result == 0; j++) {

            std::vector<Edge> edgestest = edges;

            for (int i = 1; i < numberOfPlayers; i++)
            {

                edgestest.push_back({i, numberOfPlayers + 1, j, 0});
            }
            edgestest.push_back({0, numberOfPlayers + 2, j, 0});

            edgestest.push_back({numberOfPlayers + 1, numberOfPlayers + 2, numberOfMatch - j,
                                 0});
            costMin=min_cost(edgestest, numberOfMatch, numberOfPlayers, numberOfPlayers + 2, costMax);
            if(costMin <= costMax and costMin != -1)
            {

                result=1;
                printf("TAK\n");

            }
            edgestest.clear();

        }

        if(result == 0)
        {
            printf("NIE\n");
        }
        edges.clear();
        numberOfSets= numberOfSets - 1;
    }
    return 0;
}
int min_cost(const std::vector<Edge>& edges, int numberOfFlowUnits, int s, int t, int costMax) {
    int numberOfVertices = t + 2;
    std::vector<std::vector<int>>weight;
    std::vector<std::vector<int>> vektor;
    std::vector<std::vector<int>>costVektor;
    std::queue<int> queue;
    std::vector<int> k;
    std::vector<int> w;

    costVektor.assign(numberOfVertices, std::vector<int>(numberOfVertices, 0));
    weight.assign(numberOfVertices, std::vector<int>(numberOfVertices, 0));
    vektor.assign(numberOfVertices, std::vector<int>());

    int currentFlow = 0;
    int cost = 0;

    for (int i = 0; i < edges.size(); i++) {

        vektor[edges[i].x].push_back(edges[i].y);
        vektor[edges[i].y].push_back(edges[i].x);
        costVektor[edges[i].x][edges[i].y] = edges[i].koszt;
        costVektor[edges[i].y][edges[i].x] = -edges[i].koszt;
        weight[edges[i].x][edges[i].y] = edges[i].waga;
    }

    while (currentFlow < numberOfFlowUnits) {
        std::vector<bool> vector(numberOfVertices, false);
        k.assign(numberOfVertices, INT32_MAX);
        k[s] = 0;
        int x = numberOfFlowUnits - currentFlow;
        int z = t;
        int front;
        queue.push(s);
        w.assign(numberOfVertices, INT32_MAX);
        while (!queue.empty()) {
            front = queue.front();
            vector[front] = false;
            queue.pop();
            for (int i = 0; i < vektor[front].size(); i++) {
                if (weight[front][vektor[front][i]] > 0) {
                    if (k[vektor[front][i]] > k[front] + costVektor[front][vektor[front][i]]) {
                        k[vektor[front][i]] = k[front] + costVektor[front][vektor[front][i]];
                        w[vektor[front][i]] = front;
                        if (vector[vektor[front][i]] != true) {
                            vector[vektor[front][i]] = true;
                            queue.push(vektor[front][i]);
                        }
                    }
                }
            }
        }
        if (k[t] == INT32_MAX)
            break;
        while (z != s) {
            x = std::min(x, weight[w[z]][z]);
            z = w[z];
        }
        cost += x * k[t];
        if(cost > costMax)
        {
            return -1;
        }
        currentFlow += x;
        z = t;
        while (z != s) {
            weight[w[z]][z] -= x;
            weight[z][w[z]] += x;
            z = w[z];
        }
    }
    k.clear();
    w.clear();
    if (currentFlow < numberOfFlowUnits)
        return -1;
    else
        return cost;
}
document.addEventListener('DOMContentLoaded', () => 
    {
    const chooseButton = document.getElementById('chooseButton');
    chooseButton.addEventListener('click', handleMenuChoice);

    function handleMenuChoice() 
    {
        const choice = document.getElementById('algorithm').value;
        const contentDiv = document.getElementById('content');
        contentDiv.innerHTML = '';

        switch (choice) 
        {
            case 'RR':
                displayForm('RR');
                break;
            case 'SRF':
                displayForm('SRF');
                break;
            default:
                contentDiv.innerHTML = '<p class="text-red-500">Pilihan tidak valid. Silakan pilih algoritma.</p>';
        }
    }

    function displayForm(algorithm) 
    {
        const form = `
            <label for="arrivalTimes" class="block text-gray-700">Arrival Times:</label>
            <input type="text" id="arrivalTimes" class="block w-full mt-1 mb-2" placeholder="e.g. 0 3 5 8 10">
            <label for="burstTimes" class="block text-gray-700">Burst Times:</label>
            <input type="text" id="burstTimes" class="block w-full mt-1 mb-2" placeholder="e.g. 4 7 3 9 3">
            ${algorithm === 'RR' ? '<label for="timeQuantum" class="block text-gray-700">Time Quantum:</label><input type="text" id="timeQuantum" class="block w-full mt-1 mb-2" placeholder="e.g. 3">' : ''}
            <button id="solveButton" class="bg-green-500 text-white px-4 py-2 rounded">Solve</button>
        `;
        document.getElementById('content').innerHTML = form;

        document.getElementById('solveButton').addEventListener('click', () => 
            {
            if (algorithm === 'RR')
                RoundRobin();
            else if (algorithm === 'SRF')
                ShortestRemainingFirst();
        });
    }

    function RoundRobin() 
    {
        const arrivalTimes = document.getElementById('arrivalTimes').value.trim().split(' ').map(Number);
        const burstTimes = document.getElementById('burstTimes').value.trim().split(' ').map(Number);
        const timeQuantum = Number(document.getElementById('timeQuantum').value);
        const n = arrivalTimes.length;

        let processes = arrivalTimes.map((at, index) => ({
            pid: index + 1,
            at,
            bt: burstTimes[index],
            ct: 0,
            tat: 0,
            wt: 0,
            rt: -1,
            remainingBt: burstTimes[index]
        }));

        processes.sort((a, b) => a.at - b.at);

        let currentTime = processes[0].at;
        let completed = 0;
        let isCompleted = new Array(n).fill(false);
        const ganttChart = [];

        while (completed !== n) 
            {
            let executed = false;
            for (let i = 0; i < n; i++) 
                {
                if (processes[i].remainingBt > 0 && processes[i].at <= currentTime && !isCompleted[i]) {
                    executed = true;
                    if (processes[i].rt === -1)
                        processes[i].rt = currentTime - processes[i].at;

                    ganttChart.push({ pid: processes[i].pid, start: currentTime });

                    if (processes[i].remainingBt <= timeQuantum) 
                    {
                        currentTime += processes[i].remainingBt;
                        processes[i].remainingBt = 0;
                        processes[i].ct = currentTime;
                        isCompleted[i] = true;
                        completed++;

                        processes[i].tat = processes[i].ct - processes[i].at;
                        processes[i].wt = processes[i].tat - processes[i].bt;
                    } 
                    else 
                    {
                        currentTime += timeQuantum;
                        processes[i].remainingBt -= timeQuantum;
                    }

                    ganttChart[ganttChart.length - 1].end = currentTime;
                }
            }

            if (!executed) 
            {
                let nextArrival = Math.min(...processes.filter(p => p.remainingBt > 0 && p.at > currentTime).map(p => p.at));
                currentTime = nextArrival !== Infinity ? nextArrival : currentTime + 1;
            }
        }

        displayGanttChart(ganttChart);
        displayTable(processes);
    }

    function ShortestRemainingFirst() {
        const arrivalTimes = document.getElementById('arrivalTimes').value.trim().split(' ').map(Number);
        const burstTimes = document.getElementById('burstTimes').value.trim().split(' ').map(Number);
        const n = arrivalTimes.length;
    
        let processes = arrivalTimes.map((at, index) => ({
            pid: index + 1,
            at,
            bt: burstTimes[index],
            ct: 0,
            tat: 0,
            wt: 0,
            rt: -1,
            remainingBt: burstTimes[index],
            started: false
        }));
    
        processes.sort((a, b) => a.at - b.at);
    
        let completed = 0;
        let currentTime = 0;
        let minRemainingTime = Infinity;
        let shortest = -1;
        let check = false;
        let ganttChart = [];
    
        while (completed !== n) {
            for (let j = 0; j < n; j++) {
                if ((processes[j].at <= currentTime) && 
                    ((processes[j].remainingBt < minRemainingTime) || 
                    (processes[j].remainingBt === minRemainingTime && !processes[j].started)) && 
                    processes[j].remainingBt > 0) {
                    minRemainingTime = processes[j].remainingBt;
                    shortest = j;
                    check = true;
                }
            }
    
            if (!check) {
                currentTime++;
                continue;
            }
    
            if (processes[shortest].remainingBt === processes[shortest].bt) {
                processes[shortest].rt = currentTime - processes[shortest].at;
            }
    
            let lastGanttBlock = ganttChart[ganttChart.length - 1];
            if (!lastGanttBlock || lastGanttBlock.pid !== processes[shortest].pid) {
                if (lastGanttBlock) {
                    lastGanttBlock.end = currentTime;
                }
                ganttChart.push({ pid: processes[shortest].pid, start: currentTime });
            }
    
            processes[shortest].remainingBt--;
            processes[shortest].started = true;
            minRemainingTime = processes[shortest].remainingBt;
            currentTime++;
    
            if (processes[shortest].remainingBt === 0) {
                completed++;
                check = false;
                minRemainingTime = Infinity;
    
                processes[shortest].ct = currentTime;
                processes[shortest].tat = processes[shortest].ct - processes[shortest].at;
                processes[shortest].wt = processes[shortest].tat - processes[shortest].bt;
    
                ganttChart[ganttChart.length - 1].end = currentTime;
            }
        }
    
        displayGanttChart(ganttChart);
        displayTable(processes);
    }
    
    function displayGanttChart(ganttChart) {
        const ganttChartDiv = document.getElementById('ganttChart');
        ganttChartDiv.innerHTML = '';
        ganttChart.forEach(block => {
            const ganttBlock = document.createElement('div');
            ganttBlock.className = 'gantt-block';
            ganttBlock.innerText = `P${block.pid}: ${block.start}-${block.end}`;
            ganttChartDiv.appendChild(ganttBlock);
        });
    }
    
    function displayTable(processes) {
        const outputTable = document.getElementById('outputTable');
        outputTable.innerHTML = `
            <table class="table-auto w-full text-left bg-white rounded-lg shadow">
                <thead>
                    <tr>
                        <th class="px-4 py-2 border">PID</th>
                        <th class="px-4 py-2 border">Arrival Time (AT)</th>
                        <th class="px-4 py-2 border">Burst Time (BT)</th>
                        <th class="px-4 py-2 border">Completion Time (CT)</th>
                        <th class="px-4 py-2 border">Turnaround Time (TAT)</th>
                        <th class="px-4 py-2 border">Waiting Time (WT)</th>
                        <th class="px-4 py-2 border">Response Time (RT)</th>
                    </tr>
                </thead>
                <tbody>
                    ${processes.map(p => `
                        <tr>
                            <td class="px-4 py-2 border">${p.pid}</td>
                            <td class="px-4 py-2 border">${p.at}</td>
                            <td class="px-4 py-2 border">${p.bt}</td>
                            <td class="px-4 py-2 border">${p.ct}</td>
                            <td class="px-4 py-2 border">${p.tat}</td>
                            <td class="px-4 py-2 border">${p.wt}</td>
                            <td class="px-4 py-2 border">${p.rt}</td>
                        </tr>
                    `).join('')}
                </tbody>
            </table>
        `;
    }
            
});
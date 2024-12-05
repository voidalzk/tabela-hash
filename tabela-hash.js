const votedVoters = {}; 
const voteCounts = {}; 

function vote(voterId, candidateName) {
    if (votedVoters[voterId]) {
        console.log("Erro: Eleitor já votou.");
        return;
    }

    votedVoters[voterId] = true;

    if (voteCounts[candidateName]) {
        voteCounts[candidateName] += 1;
    } else {
        voteCounts[candidateName] = 1;
    }

    console.log(`Voto registrado para ${candidateName}!`);
}

function show_results() {
    console.log("Resultados da Eleição:");
    for (const candidate in voteCounts) {
        console.log(`${candidate}: ${voteCounts[candidate]} votos`);
    }
}


vote("123", "Alice");
vote("123", "Bob");
vote("456", "Bob");  
vote("789", "Alice"); 
vote("101", "Carlos");

show_results();

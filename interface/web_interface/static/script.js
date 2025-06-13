function sendCommand(command) {
  console.log(`📤 Enviando comando: ${command}`);

  fetch('/command', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ command: command })
  })
    .then(response => {
      console.log('✅ Resposta recebida do servidor (POST /command)');
      return response.json();
    })
    .then(data => {
      console.log('📥 Dados da resposta:', data);
      document.getElementById('last-response').innerText = `> ${data.response}`;
      if (command === 'status') {
        console.log('🔄 Atualizando status após comando status');
        updateStatus();
      }
    })
    .catch(error => {
      console.error('❌ Erro ao enviar comando:', error);
    });
}

function updateStatus() {
  console.log('🔄 Requisição GET /status iniciada');

  fetch('/status')
    .then(response => {
      console.log('✅ Resposta recebida do servidor (GET /status)');
      return response.json();
    })
    .then(data => {
      console.log('📥 Dados do status:', data);
      const text = data.status;
      const status = parseStatus(text);
      document.getElementById('moisture').innerText = status.moisture;
      document.getElementById('reservoir').innerText = status.reservoir;
      document.getElementById('pump').innerText = status.pump;
      document.getElementById('system').innerText = status.system;
      document.getElementById('activations').innerText = status.activations;
    })
    .catch(error => {
      console.error('❌ Erro ao obter status:', error);
    });
}

function parseStatus(text) {
  console.log('🔍 Parseando texto do status:', text);

  const status = {
    moisture: '--%',
    reservoir: '---',
    pump: '---',
    system: '---',
    activations: '0'
  };
  const parts = text.split(';');
  parts.forEach(p => {
    const [key, value] = p.split('=');
    if (status.hasOwnProperty(key)) status[key] = value;
  });
  return status;
}

// Atualiza status ao carregar a página
window.addEventListener('DOMContentLoaded', () => {
  console.log('🌐 Página carregada. Atualizando status inicial...');
  updateStatus();
});

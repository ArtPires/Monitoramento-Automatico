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
      if (!response.ok) throw new Error('Resposta não OK');
      return response.json();
    })
    .then(data => {
      console.log('📥 Dados do status:', data);

      // Umidade do solo
      document.getElementById('moisture').innerText = data.systemStatus.moisture + '%';

      // Nível do reservatório
      document.getElementById('reservoir').innerText = data.systemStatus.waterLevel + '%';

      // Status da bomba
      document.getElementById('pump').innerText = data.waterPump.status;

      // Status do sistema
      document.getElementById('system').innerText = data.systemStatus.status;

      // Última ativação da bomba: convertendo timestamp para data legível
      const lastTime = new Date(data.waterPump.lastTimeOk * 1000); // timestamp Unix em ms
      document.getElementById('activations').innerText = lastTime.toLocaleString();
    })
    .catch(error => {
      console.error('❌ Erro ao obter status:', error);
      document.getElementById('moisture').innerText = '--%';
      document.getElementById('reservoir').innerText = '---';
      document.getElementById('pump').innerText = '---';
      document.getElementById('system').innerText = '---';
      document.getElementById('activations').innerText = '---';
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

window.addEventListener('DOMContentLoaded', () => {
  console.log('🌐 Página carregada. Atualizando status inicial...');
  updateStatus(); // primeira atualização

  // atualiza a cada 5 minutos
  setInterval(updateStatus, 5 * 60 * 1000);
});
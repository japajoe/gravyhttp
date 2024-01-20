let responseArea;
let csrfToken

document.addEventListener('DOMContentLoaded', function () {
    responseArea = document.getElementById('responseArea');
    csrfToken = document.querySelector('input[type="hidden"][name="csrf-token"]').value;
    const button = document.getElementById('buttonSend');

    if (button) {
        button.addEventListener('click', function () {
            onButtonClick();
        });
    }
});

function onButtonClick() {
    const url = '/api/v1/example';

    const data = {
        token: csrfToken,
        key1: 'value1',
        key2: 'value2'
    };

    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    .then(response => {
        if (!response.ok) {
            throw response.status;
        }
        return response.json(); // Parse the JSON response
    })
    .then(data => {
        if (responseArea) {
            responseArea.innerText = JSON.stringify(data);
        } else {
            console.log('POST request successful:', data);            
        }
    })
    .catch(error => {
        if (responseArea) {
            responseArea.innerText = 'Error status: ' + error;
        } else {
            console.error('Error during POST request:', error);
        }
    });
}
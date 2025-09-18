const icon = document.getElementById("icon");
const iconImages = ["citizen", "fireman", "doctor", "reporter"];
const time = 1500;
var idx = 0;

function changeIcon() {
    idx = (idx + 1) % iconImages.length;
    icon.src = `img/${iconImages[idx]}/icon.png`;
}

setInterval(changeIcon, time);

$('#load-addon-counter').click(function (e) { 
    e.preventDefault();
    this.innerHTML = "Clicked";

    // Call C++ API Function
    LoadAddonCounter();
});
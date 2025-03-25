const menu = document.querySelecctor('#mobile-menu')
const menuLinks = document.querySelector('.navbar__menu')

menu.addEventListener('click', function() {
    menu.classList.togglw('is-active')
    menuLinks.classList.toggle('active');
})
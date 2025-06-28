
document.addEventListener('scroll', () => {
	const header  = document.getElementById('site_header')
	const logo    = document.getElementById('logo')
	const title   = document.getElementById('homepage_title')
	const navbar  = document.getElementById('navbar')
	const socials = document.getElementById('socials')

	if (window.scrollY > 0) {
		header.classList.add('scrolled_header')
		title.classList.add('scrolled_header_element')
		navbar.classList.add('scrolled_header_element')
		logo.classList.add('scrolled_header_img')
		socials.classList.add('scrolled_header_img2')
	} else {
		header.classList.remove('scrolled_header')
		title.classList.remove('scrolled_header_element')
		navbar.classList.remove('scrolled_header_element')
		logo.classList.remove('scrolled_header_img')
		socials.classList.remove('scrolled_header_img2')
	}
})

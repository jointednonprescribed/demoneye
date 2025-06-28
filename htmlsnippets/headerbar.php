
<header id="site_header">
	<?php
		echo '<link rel="stylesheet" href="', $ROOTDIR, '/htmlsnippets/css/headerbar.css">';
		echo '<script src="', $ROOTDIR, '/htmlsnippets/js/headerbar.js"></script>';
	?>
	<!-- Top-of-Page Navigation Bar -->
	<!-- Logo/Home Page Link -->
	<nav>
		<element id="logo">
		<?php
			echo '<a href="', $ROOTDIR, '"><img alt="Home" src="', $ROOTDIR, '/res/logo.svg" width="90%" height="90%"></a>';
		?>
		</element>
		<element id="homepage_title">
			<?php
				echo '<a href="', $ROOTDIR, '"><p style="font-size: 22px;">Demoneye</p><p style="font-size: 13px;">Engine & SDK</p></a>';
			?>
		</element>
	</nav>

	<!-- General Navbar -->
	<nav id="navbar">
		<?php
			echo '<a href="', $ROOTDIR, '/guides/">Setup & Development Guides</a>';
			echo '<a href="', $ROOTDIR, '/downloads/">Downloads</a>';
			echo '<a href="', $ROOTDIR, '/support/">Support & Feedback</a>';
		?>
	</nav>

	<!-- Social Media Navbar -->
	<div id="socials">
		<?php
			# Developer's Website
			echo '<a target="_blank" href="https://jointednonprescribed.github.io"><img alt="Developer\'s Website" src="', $ROOTDIR, '/res/code-slash.svg" width="30" height="30"></a>';
			# GitHub
			echo '<a target="_blank" href="https://github.com/jointednonprescribed/demoneye"><img alt="GitHub" src="', $ROOTDIR, '/res/github.svg" width="30" height="30"></a>';
			# YouTube
			echo '<a target="_blank" href="https://youtube.com/"><img alt="YouTube" src="', $ROOTDIR, '/res/youtube.svg" width="30" height="30"></a>';
		?>
	</div>
</header>

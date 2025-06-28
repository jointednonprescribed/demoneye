
<!DOCTYPE html>
<html>
	<head>
		<title>Example Title</title>
		<meta name="description" content="Example description.">

		<!-- HTML Snippets -->
		<?php
			$DOCROOT = '..';
			include $DOCROOT . '/rootmgr.php';

			# Include Default Page Metadata
			include $DOCROOT . '/intern/pagemeta.html';

			# Download Fonts...
			include $DOCROOT . '/extern/googlefonts.html';
			
			# CSS/JS Snippets
			
			# 'Headerbar' snippet
			echo '<link rel="stylesheet" href="', $DOCROOT, '/htmlsnippets/css/headerbar.css">';
			echo '<script src="', $DOCROOT, '/htmlsnippets/js/headerbar.js"></script>';
		?>
	</head>

	<body>

		<?php
			include $DOCROOT . '/htmlsnippet/headerbar.php'
		?>

	</body>
</html>

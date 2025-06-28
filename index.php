
<!DOCTYPE html>
<html>
	<head>
		<title>Demoneye Engine & SDK</title>
		<meta name="description" content="Demoneye Engine and SDK Official Website">

		<!-- HTML Includes -->
		<?php
			$ROOTDIR = '.';
			$NAVBAR_SEL = 0;

			# Include Default Page Metadata
			include './intern/pagemeta.html';

			# Download Fonts...
			include './extern/googlefonts.html';
		?>

		<link rel="stylesheet" href="./css/default.css">
		<link rel="stylesheet" href="./css/index.css">
	</head>
	<body>
		<?php
			include './htmlsnippets/headerbar.php';
		?>

		<div class="textpage" id="latest_dl">
			<h1 style="line-spacing: 1px;">Latest Downloads</h1>
			<hr>
			<!-- <p id="legal_notice">
				Ownership of icons below belong, respectively, to the following authors:
				<ul>
					<li>Microsoft Corporation</li>
					<li>Larry Ewing, Simon Budig, and Garrett LeSage and the GIMP
						(<nav><a href="legal/linux.svg.d/README.txt" target="_blank">README</a></nav>)</li>
					<li></li>
				</ul>
			</p> -->
			<table id="dlbutton_container">
				<tr>
					<th class="borderless-cell"></th>
					<th>Windows</th>
					<th>Linux</th>
					<th>MacOSX</th>
				</tr>
				<tr>
					<nav id="experimental_dls" class="dl_link_container">
					<td>Experimental Builds</td>
					<td>
						<a target="_blank" href="downloads/files/latest/exp/windows-x86.zip" download>x86</a> |
						<a target="_blank" href="downloads/files/latest/exp/windows-amd64.zip" download>amd64</a> |
						<a target="_blank" href="downloads/files/latest/exp/windows-arm.zip" download>arm</a> |
						<a target="_blank" href="downloads/files/latest/exp/windows-aarch64.zip" download>aarch64</a>
					</td>
					<td>
						<a target="_blank" href="downloads/files/latest/exp/linux-x86.zip" download>x86</a> |
						<a target="_blank" href="downloads/files/latest/exp/linux-amd64.zip" download>amd64</a> |
						<a target="_blank" href="downloads/files/latest/exp/linux-arm.zip" download>arm</a> |
						<a target="_blank" href="downloads/files/latest/exp/linux-aarch64.zip" download>aarch64</a>
					</td>
					<td>
						<a target="_blank" href="downloads/files/latest/exp/osx-x86.zip" download>x86</a> |
						<a target="_blank" href="downloads/files/latest/exp/osx-amd64.zip" download>amd64</a> |
						<a target="_blank" href="downloads/files/latest/exp/osx-arm.zip" download>arm</a> |
						<a target="_blank" href="downloads/files/latest/exp/osx-aarch64.zip" download>aarch64</a>
					</td>
					</nav>
				</tr>

				<tr>
					<nav id="stable_dls" class="dl_link_container">
					<td>Stable Builds</td>
					<td>
						<a target="_blank" href="downloads/files/latest/stable/windows-x86.zip" download>x86</a> |
						<a target="_blank" href="downloads/files/latest/stable/windows-amd64.zip" download>amd64</a> |
						<a target="_blank" href="downloads/files/latest/stable/windows-arm.zip" download>arm</a> |
						<a target="_blank" href="downloads/files/latest/stable/windows-aarch64.zip" download>aarch64</a>
					</td>
					<td>
						<a target="_blank" href="downloads/files/latest/stable/linux-x86.zip" download>x86</a> |
						<a target="_blank" href="downloads/files/latest/stable/linux-amd64.zip" download>amd64</a> |
						<a target="_blank" href="downloads/files/latest/stable/linux-arm.zip" download>arm</a> |
						<a target="_blank" href="downloads/files/latest/stable/linux-aarch64.zip" download>aarch64</a>
					</td>
					<td>
						<a target="_blank" href="downloads/files/latest/stable/osx-x86.zip" download>x86</a> |
						<a target="_blank" href="downloads/files/latest/stable/osx-amd64.zip" download>amd64</a> |
						<a target="_blank" href="downloads/files/latest/stable/osx-arm.zip" download>arm</a> |
						<a target="_blank" href="downloads/files/latest/stable/osx-aarch64.zip" download>aarch64</a>
					</td>
					</nav>
				</tr>
			</table>

			<nav><a href="downloads" id="see_all_dls">See All Downloads</a></nav>

			<img id="img_win" src="res/windows.svg" width=20 height=20>
			<img id="img_linux" src="res/Tux.svg" width=20 height=20>
			<img id="img_osx" src="res/osx.svg" width=20 height=20>
		</div>

		

	</body>
</html>

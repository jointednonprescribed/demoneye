
<!DOCTYPE html>
<html>
	<head>
		<title>Downloads | Demoneye Engine & SDK</title>
		<meta name="description" content="Setup Guides and Development Guides for both Demoneye Engine and Demoneye SDK. See this page for any usage or setup documentumentation.">

		<!-- HTML Snippets -->
		<?php
			$ROOTDIR = '..';
			$NAVBAR_SEL = 2;

			# Include Default Page Metadata
			include '../intern/pagemeta.html';

			# Download Fonts...
			include '../extern/googlefonts.html';
		?>

		<link rel="stylesheet" href="/css/default.css">
		<link rel="stylesheet" href="css/index.css">
	</head>
	<body>
		<?php
			include '../htmlsnippets/headerbar.php';
		?>

		<div class="textpage">
			<h1>Downloads</h1>
			<hr>

			<p style="opacity: 0.4; font-size: 0.8vw;">Only direct downloads are available.</p>

			<?php
				if (isset($_GET['platform']) && in_array($_GET['platform'], ["windows", "linux", "osx"])) {
					echo '<scrpt type="application/javascript">const PLATFORM_SELECTION = "' . $_GET['platform'] . '"</script>';
				} else {
					echo '<script type="application/javascript">const PLATFORM_SELECTION = null</script>';
				}
			?>

			<select id="dltable_platform_selector" onchange="set_dltable_to(this.value)">
				<option selected="selected" value="unset">Select Platform...</option>
				<option value="windows">Windows</option>
				<option value="linux">Linux</option>
				<option value="osx">MacOSX</option>
			</select>

			<div class="dltable_pair" id="dltable_pair_windows">
				<div class="dltable_header_pair" id="dltable_header_pair_sdk_windows">
				<h2 class="dltable_header dltable_header_sdk" id="dltable_header_sdk_windows">Source Development Kit</h2>
				<table class="dltable" id="dltable_sdk_windows">
					<th>Archive Download</th><th>Installer Download</th><th>Version</th><th>Stable or Experimental</th>
					<?php
						$_ARCH_X86             = "x86";
						$_ARCH_AMD64           = "amd64";
						$_ARCH_ARM             = "arm";
						$_ARCH_AARCH64         = "aarch64";

						$_PLATFORM_WINDOWS     = "windows";
						$_PLATFORM_LINUX       = "linux";
						$_PLATFORM_OSX         = "osx";
						$_PLATFORM_ANDROID     = "android";
						$_PLATFORM_IOS         = "ios";

						$_BASENAME_SDK         = "demoneye-sdk";
						$_BASENAME_ENGINE      = "demoneye-engine";
						$_BASENAME_SDK_INST    = "demoneye-sdk-inst";
						$_BASENAME_ENGINE_INST = "demoneye-engine-inst";

						function versionstr($year, int $major, int $minor, int $fix): string {
							if ($year == null || !is_int($year)) {
								return strval($major) . '.' . strval($minor) . '.' . strval($fix);
							} else {
								return strval($year) . '.' . strval($major) . '.' . strval($minor) . '.' . strval($fix);
							}
						}

						# File Name Format:
						#   <base-name>-<version>-<platform>_<architecture>.(extension-corresponding-to-platform)
						function gen_filename(string $fileBaseName, string $version, string $platform, string $arch, bool $installer=false): string {
							if ( // none of these variables can be null 
								$fileBaseName == null ||
								$version == null ||
								$platform == null ||
								$arch == null
							) return null;

							$filename = $fileBaseName . '-' . $version . '-' . $platform . '_' . $arch;

							if ($platform == $_PLATFORM_WINDOWS) {
								$filename = $filename . '.zip';
							} else if ($platform == $_PLATFORM_LINUX) {
								$filename = $filename . '.tar.gz';
							} else if ($platform == $_PLATFORM_OSX) {
								if ($installer) {
									$filename = $filename . '.pkg';
								} else {
									$filename = $filename . '.zip';
								}
							} else
								return null;
						}

						function gen_row(string $fileBaseName, string $version, string $platform, bool $stable=true) {
							$filename_x86     = gen_filename($fileBaseName, $version, $platform, _ARCH_X86, false);
							$filename_amd64   = gen_filename($fileBaseName, $version, $platform, _ARCH_AMD64, false);
							$filename_arm     = gen_filename($fileBaseName, $version, $platform, _ARCH_ARM, false);
							$filename_aarch64 = gen_filename($fileBaseName, $version, $platform, _ARCH_AARCH64, false);

							$filename_inst_x86      = gen_filename($fileBaseName . '-inst', $version, $platform, _ARCH_X86, true);
							$filename_inst_amd64    = gen_filename($fileBaseName . '-inst', $version, $platform, _ARCH_AMD64, true);
							$filename_inst_arm      = gen_filename($fileBaseName . '-inst', $version, $platform, _ARCH_ARM, true);
							$filename_inst_aarch64  = gen_filename($fileBaseName . '-inst', $version, $platform, _ARCH_AARCH64, true);

							echo '<tr><td><a target="_blank" download href="files/' . $filename_x86 . '">x86</a> | 
								<a target="_blank" download href="files/' . $filename_amd64 . '">amd64</a> | 
								<a target="_blank" download href="files/' . $filename_arm . '">arm</a> | 
								<a target="_blank" download href="files/' . $filename_aarch64 . '">aarch64</a></td>
								<td><a target="_blank" download href="files/' . $filename_inst_x86 . '">x86</a> | 
								<a target="_blank" download href="files/' . $filename_inst_amd64 . '">amd64</a> | 
								<a target="_blank" download href="files/' . $filename_inst_arm . '">arm</a> | 
								<a target="_blank" download href="files/' . $filename_inst_aarch64 . '">aarch64</a></td>
								<td>' . $version . '</td>';

							if ($stable)
								echo '<td>Stabled</td>';
							else
								echo '<td>Experimental</td>';
							echo '</tr>';
						}

						function gen_row_sdk(string $version, string $platform) {
							gen_row("demoneye-sdk", $version, $platform);
						}
						function gen_row_engine(string $version, string $platform) {
							gen_row("demoneye-engine", $version, $platform);
						}

						// Enter Rows...
					?>
				</table>
				</div>
				<div class="dltable_header_pair" id="dltable_header_pair_engine_windows">
				<h2 class="dltable_header dltable_header_engine" id="dltable_header_engine_windows">Engine</h2>
				<table class="dltable" id="dltable_engine_windows">
					<th>Archive Download</th><th>Installer Download</th><th>Version</th><th>Stable or Experimental</th>
					
				</table>
				</div>
			</div>

			<div class="dltable_pair" id="dltable_pair_linux">
				<div class="dltable_header_pair" id="dltable_header_pair_sdk_linux">
				<h2 class="dltable_header dltable_header_sdk" id="dltable_header_sdk_linux">Source Development Kit</h2>
				<table class="dltable" id="dltable_sdk_linux">
					<th>Archive Download</th><th>Installer Download</th><th>Version</th><th>Stable or Experimental</th>
					
				</table>
				</div>
				<div class="dltable_header_pair" id="dltable_header_pair_engine_linux">
				<h2 class="dltable_header dltable_header_engine" id="dltable_header_engine_linux">Engine</h2>
				<table class="dltable" id="dltable_engine_linux">
					<th>Archive Download</th><th>Installer Download</th><th>Version</th><th>Stable or Experimental</th>
					
				</table>
				</div>
			</div>

			<div class="dltable_pair" id="dltable_pair_osx">
				<div class="dltable_header_pair" id="dltable_header_pair_sdk_osx">
				<h2 class="dltable_header dltable_header_sdk" id="dltable_header_sdk_osx">Source Development Kit</h2>
				<table class="dltable" id="dltable_sdk_osx">
					<th>Archive Download</th><th>Installer Download</th><th>Version</th><th>Stable or Experimental</th>
					
				</table>
				</div>
				<div class="dltable_header_pair" id="dltable_header_pair_engine_osx">
				<h2 class="dltable_header dltable_header_engine" id="dltable_header_engine_osx">Engine</h2>
				<table class="dltable" id="dltable_engine_osx">
					<th>Archive Download</th><th>Installer Download</th><th>Version</th><th>Stable or Experimental</th>
					
				</table>
				</div>
			</div>
		</div>

		<script type="application/javascript" src="js/index.js" defer></script>
	</body>
</html>

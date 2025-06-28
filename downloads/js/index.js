
const USER_AGENT_STR = navigator.userAgent

var CURRENT_DLTABLE = null

const ACCEPTED_PLATFORMS = ['windows', 'linux', 'osx', 'android', 'ios'];

function set_dltable_to(next) {
	if (ACCEPTED_PLATFORMS.includes(CURRENT_DLTABLE)) {
		document.getElementById('dltable_header_sdk_' + CURRENT_DLTABLE).classList.remove('dltable_header_shown')
		document.getElementById('dltable_header_engine_' + CURRENT_DLTABLE).classList.remove('dltable_header_shown')
		document.getElementById('dltable_sdk_' + CURRENT_DLTABLE).classList.remove('dltable_shown')
		document.getElementById('dltable_engine_' + CURRENT_DLTABLE).classList.remove('dltable_shown')
	}
	if (ACCEPTED_PLATFORMS.includes(next)) {
		document.getElementById('dltable_header_sdk_' + next).classList.add('dltable_header_shown')
		document.getElementById('dltable_header_engine_' + next).classList.add('dltable_header_shown')
		document.getElementById('dltable_sdk_' + next).classList.add('dltable_shown')
		document.getElementById('dltable_engine_' + next).classList.add('dltable_shown')
	}
	CURRENT_DLTABLE = next
}

const DLTABLE_PLATFORM_SELECTOR = document.getElementById('dltable_platform_selector')

if (PLATFORM_SELECTION in ACCEPTED_PLATFORMS) {
	DLTABLE_PLATFORM_SELECTOR.value = PLATFORM_SELECTION;
	set_dltable_to(PLATFORM_SELECTION)
} else if (USER_AGENT_STR.indexOf('Windows') >= 0) {
	DLTABLE_PLATFORM_SELECTOR.value = "windows";
	set_dltable_to("windows")
} else if (USER_AGENT_STR.indexOf('Linux') >= 0) {
	DLTABLE_PLATFORM_SELECTOR.value = "linux";
	set_dltable_to("linux")
} else if (USER_AGENT_STR.indexOf('Macintosh') >= 0) {
	DLTABLE_PLATFORM_SELECTOR.value = "osx";
	set_dltable_to("osx")
}

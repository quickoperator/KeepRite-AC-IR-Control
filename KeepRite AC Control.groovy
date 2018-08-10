/**
 *  KeepRite AC Control v1.0.20180719
 *
 *  Copyright 2018 Erik Tellier
 *  Based on : Generic HTTP Device : Link to the project: https://community.smartthings.com/t/43335
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 */

import groovy.json.JsonSlurper

metadata {
	definition (name: "KeepRite AC Control", author: "Erik Tellier", namespace:"Erik Tellier") {
		capability "Switch"
        capability "Temperature Measurement"
		capability "Refresh"
        attribute "DevicePort", "string"
        attribute "DevicePath", "string"
		attribute "on", "string"
		attribute "off", "string"
		attribute "turbo", "string"
        attribute "aird", "string"
        attribute "triggerswitch", "string"
        attribute "triggerswing", "string"
        attribute "temperature", "string"
		attribute "humidity", "string"
		command "RefreshTrigger"
		command "on"
		command "off"
        command "turbo"
//        command "turboon"
//        command "turbooff"
        command "aird"
        command "SwingTrigger"
        command "swingon"
        command "swingoff"
        command "ACTrigger"
	}

	preferences {
		input("DeviceIP", "string", title:"Device IP Address", description: "Addresse IP NodeMCU", required: true, displayDuringSetup: true)
		input(name: "DeviceTempMeasurement", type: "enum", title: "Celcius ou Fahrenheit? Default Celcius.", options: ["Celcius","Fahrenheit"], defaultValue: "Celcius", required: false, displayDuringSetup: true)
        input(name: "DevicePostGet", type: "enum", title: "POST ou GET. POST pour PHP & GET pour NodeMCU.", options: ["POST","GET"], defaultValue: "GET", required: false, displayDuringSetup: true)
			}

	simulator {
	}

	tiles(scale: 2) {
    	valueTile("temperature", "device.temperature", width: 2, height: 2) {
			state("default", label:'Temp\n${currentValue}',
				backgroundColors:[
					[value: 16, color: "#153591"], //Bleu
					[value: 20, color: "#1aaab2"], //Cyan
					[value: 21, color: "#00cc33"], //Vert 
					[value: 23, color: "#006018"], //Vert foncé
					[value: 24, color: "#f1d801"], //Jaune
					[value: 25, color: "#ff6600"], //Orange
					[value: 26, color: "#ff0000"]  //Rouge
				]
			)
		}
		valueTile("humidity", "device.humidity", width: 2, height: 2) {
			state("default", label: 'Hum.\n${currentValue}',
				backgroundColors:[
					[value: 50, color: "#00cc33"], //Vert
					[value: 60, color: "#f1d801"], //Jaune
					[value: 67, color: "#ff6600"], //Orange
					[value: 75, color: "#ff0000"]  //Rouge
				]
			)
		}
		standardTile("RefreshTrigger", "device.refreshswitch", width: 2, height: 2) {
			state "default", label:'MAJ', action: "refresh.refresh", icon: "st.secondary.refresh-icon", backgroundColor:"#1aaab2", nextState: "envoi"
			state "envoi", label: 'envoi', action: "ResetTiles", icon: "st.secondary.refresh-icon", backgroundColor: "#FF6600", nextState: "default"
		}
		standardTile("on", "device.on", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "on", label: 'ON', action: "on", icon: "st.thermostat.ac.air-conditioning", backgroundColor: "#00cc33", nextState: "envoi"
			state "envoi", label: 'envoi', action: "", icon: "st.thermostat.ac.air-conditioning", backgroundColor: "#ff6600"
		}
		standardTile("off", "device.off", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "off", label:'OFF' , action: "off", icon: "st.thermostat.ac.air-conditioning", backgroundColor:"ff0000", nextState: "envoi"
			state "envoi", label: 'envoi', action: "", icon: "st.thermostat.ac.air-conditioning", backgroundColor: "#ff6600"
		}
//      standardTile("turbo", "device.turbo", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
//          state "turbooff", label:'STOP' , action: "turboon", icon: "st.Weather.weather1", backgroundColor:"#ffffff", nextState: "envoi"
//			state "turboon", label: 'Turbo', action: "turbooff", icon: "st.Weather.weather1", backgroundColor: "#00cc33", nextState: "envoi"
//			state "envoi", label: 'envoi', action: "", icon: "st.Weather.weather1", backgroundColor: "#ff6600"
//		}
        standardTile("turbo", "device.turbo", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "turbo", label: 'Turbo', action: "turbo", icon: "st.Weather.weather1", backgroundColor: "#ffffff", nextState: "envoi"
			state "envoi", label: 'envoi', action: "", icon: "st.Weather.weather1", backgroundColor: "#ff6600"
		}
        standardTile("aird", "device.aird", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
			state "aird", label:'Air Dir' , action: "aird", icon: "st.vents.vent-open", backgroundColor:"#1aaab2", nextState: "envoi"
			state "envoi", label: 'envoi', action: "", icon: "st.vents.vent-open", backgroundColor: "#ff6600"
		} 
        standardTile("SwingTrigger", "device.triggerswing", width: 2, height: 2, canChangeIcon: true, canChangeBackground: true) {
            state "triggerswingoff", label:'STOP' , action: "swingon", icon: "st.vents.vent-open", backgroundColor:"#ffffff", nextState: "envoi"
			state "triggerswingon", label: 'SWING', action: "swingoff", icon: "st.vents.vent-open", backgroundColor: "#00cc33", nextState: "envoi"
			state "envoi", label: 'envoi', action: "", icon: "st.vents.vent-open", backgroundColor: "#ff6600"
		}
        standardTile("ACTrigger", "device.triggerswitch", width: 4, height: 4, canChangeIcon: true, canChangeBackground: true) {
			state "triggeroff", label:'OFF' , action: "on", icon: "st.thermostat.ac.air-conditioning", backgroundColor:"#ff0000", nextState: "envoi"
			state "triggeron", label: 'ON', action: "off", icon: "st.thermostat.ac.air-conditioning", backgroundColor: "#00cc33", nextState: "envoi"
			state "envoi", label: 'envoi', action: "", icon: "st.thermostat.ac.air-conditioning", backgroundColor: "#ff6600"
        }
		main "ACTrigger"
        //main "temperature"
		details(["temperature", "humidity", "RefreshTrigger", "on", "off", "turbo", "ACTrigger", "aird", "SwingTrigger"])
        
	}
}

def refresh() {
	def FullCommand = 'Refresh='
	runCmd(FullCommand)
}

def poll() {
	refresh()
}

def ping() {
    log.debug "ping()"
	refresh()
}

def on() {
	sendEvent(name: "on", value: "on", isStateChange: true)
    sendEvent(name: "triggerswitch", value: "triggeron", isStateChange: true)
    state.ac = "on";
	runCmdir("/ir?ac=on")
}

def off() {
	sendEvent(name: "off", value: "off", isStateChange: true)
    sendEvent(name: "triggerswitch", value: "triggeroff", isStateChange: true)
    state.ac = "off";
	runCmdir("/ir?ac=off")
}

//def turboon() {
//	//log.debug "turbo On!!!"
//    sendEvent(name: "turbo", value: "turboon", isStateChange: true)
//    state.ac = "Turbo On";
//	runCmdir("/ir?ac=turbo")
//}

//def turbooff() {
//    sendEvent(name: "turbo", value: "turbooff", isStateChange: true)
//    state.ac = "Turbo Off";
//	runCmdir("/ir?ac=turbo")
//}

def turbo() {
    sendEvent(name: "turbo", value: "turbo", isStateChange: true)
    state.ac = "Turbo";
	runCmdir("/ir?ac=turbo")
}

def aird() {
    sendEvent(name: "aird", value: "aird", isStateChange: true)
    state.ac = "aird";
	runCmdir("/ir?ac=aird")
}

def swingon() {
    sendEvent(name: "triggerswing", value: "triggerswingon", isStateChange: true)
    state.ac = "swing on";
	runCmdir("/ir?ac=swing")
}

def swingoff() {
    sendEvent(name: "triggerswing", value: "triggerswingoff", isStateChange: true)
    state.ac = "swing off";
	runCmdir("/ir?ac=swing")
}

def runCmd(String varCommand) { //Commande de lecture Temp & humidité sur le port 80
	def host = DeviceIP
	def hosthex = convertIPtoHex(host).toUpperCase()
    def LocalDevicePort = "80"
	def porthex = convertPortToHex(LocalDevicePort).toUpperCase()
	device.deviceNetworkId = "$hosthex:$porthex"

	log.debug "Device ID hex = $device.deviceNetworkId"

	def headers = [:] 
	headers.put("HOST", "$host:$LocalDevicePort")
	headers.put("Content-Type", "application/x-www-form-urlencoded")

	log.debug "NodeMCU IP:Port = $headers"

	def path = ''
    def body = ''
	def method = "POST"
	try {
		if (DevicePostGet.toUpperCase() == "GET") {
			method = "GET"
			path = varCommand
			if (path.substring(0,1) != "/") { path = "/" + path }
			log.debug "Commande de lecture= $path"
		} else {
			path = DevicePath
            body = varCommand 
			log.debug "POST body is: $body"
		}
	}
	catch (Exception e) {
		settings.DevicePostGet = "POST"
		log.debug e
		log.debug "You must not have set the preference for the DevicePOSTGET option"
	}

	try {
		def hubAction = new physicalgraph.device.HubAction(
			method: method,
			path: path,
			body: body,
			headers: headers
			)
		hubAction.options = [outputMsgToS3:false]
		log.debug hubAction
		hubAction
	}
	catch (Exception e) {
		log.debug "Hit Exception $e on $hubAction"
	}
}

def runCmdir(String varCommand) { //Commande d'envoi de signaux IR sur le port 81
	def host = DeviceIP
	def hosthex = convertIPtoHex(host).toUpperCase()
    def LocalDevicePort = "81"
	def porthex = convertPortToHex(LocalDevicePort).toUpperCase()
	device.deviceNetworkId = "$hosthex:$porthex"

	log.debug "Device ID hex = $device.deviceNetworkId"

	def headers = [:] 
	headers.put("HOST", "$host:$LocalDevicePort")
	headers.put("Content-Type", "application/x-www-form-urlencoded")

	log.debug "NodeMCU IP:Port = $headers"

	def path = ''
    def body = ''
	def method = "POST"
	try {
		if (DevicePostGet.toUpperCase() == "GET") {
			method = "GET"
			path = varCommand
			if (path.substring(0,1) != "/") { path = "/" + path }
			log.debug "Commande IR= $path"
		} else {
			path = DevicePath
            body = varCommand 
			log.debug "POST body is: $body"
		}
	}
	catch (Exception e) {
		settings.DevicePostGet = "POST"
		log.debug e
		log.debug "You must not have set the preference for the DevicePOSTGET option"
	}

	try {
		def hubAction = new physicalgraph.device.HubAction(
			method: method,
			path: path,
			body: body,
			headers: headers
			)
		hubAction.options = [outputMsgToS3:false]
		log.debug hubAction
		hubAction
	}
	catch (Exception e) {
		log.debug "Hit Exception $e on $hubAction"
	}
}

def parse(String description) {
	def whichTile = ''	
   	def map = [:]
	def retResult = []
	def descMap = parseDescriptionAsMap(description)
	def jsonlist = [:]
	def bodyReturned = ' '
	def headersReturned = ' '
	if (descMap["body"]) { bodyReturned = new String(descMap["body"].decodeBase64()) }
	if (descMap["headers"]) { headersReturned = new String(descMap["headers"].decodeBase64()) }
	if (descMap["body"]) {
			jsonlist.put ("Date", new Date().format("yyyy-MM-dd h:mm:ss a", location.timeZone))
			def data=bodyReturned.eachLine { line ->
            //log.debug "DECODE TEMP"
				if (line.contains('Temperature=')) { jsonlist.put ("Temperature",line.replace("Temperature=","").replaceAll("[^\\p{ASCII}]", "°")) }
				if (line.contains('Humidity=')) { jsonlist.put ("Humidity",line.replace("Humidity=","")) }
				if (line.contains('Refresh=')) { jsonlist.put ("Refresh", "Success") }
				if (line.contains('Refresh=Success')) { jsonlist.put ("Refresh", "Success") }
				if (line.contains('Refresh=Failed : Authentication Required!')) { jsonlist.put ("Refresh", "Authentication Required!") }
			}
	}
	if (descMap["body"]) {

		if (jsonlist."Refresh"=="Success") {
			sendEvent(name: "refreshTriggered", value: jsonlist."Date", unit: "")
			whichTile = 'refresh'
		}
		if (jsonlist."Temperature") {
			if (DeviceTempMeasurement == "Celcius") {
				sendEvent(name: "temperature", value: jsonlist."Temperature".split(" ")[0].replace("°C"," ").replace("\'C"," "), unit: "")
			} else {
				sendEvent(name: "temperature", value: jsonlist."Temperature".split(" ")[1].replace("°F"," ").replace("\'F"," "), unit: "")
			}
		} 
		if (jsonlist."Humidity") {
			sendEvent(name: "humidity", value: jsonlist."Humidity".replace("=","\n"), unit: "")
		}
	}

	log.debug jsonlist

	//CHANGE NAME TILE
	sendEvent(name: "displayName", value: DeviceIP, unit: "")

	//RETURN BUTTONS TO CORRECT STATE & SET EPOCH AGAIN
	log.debug 'whichTile: ' + whichTile
    switch (whichTile) {
        case 'refresh':
			sendEvent(name: "refreshswitch", value: "default", isStateChange: true)
			return createEvent(name: "refreshswitch", value: "default", isStateChange: true)
        default:
			sendEvent(name: "refreshswitch", value: "default", isStateChange: true)
			return createEvent(name: "refreshswitch", value: "default", isStateChange: true)
			//log.debug "refreshswitch returned ${result?.descriptionText}"
    }	
}

def parseDescriptionAsMap(description) {
	description.split(",").inject([:]) { map, param ->
	def nameAndValue = param.split(":")
	map += [(nameAndValue[0].trim()):nameAndValue[1].trim()]
	}
}

private String convertIPtoHex(ipAddress) {
	String hex = ipAddress.tokenize( '.' ).collect {  String.format( '%02x', it.toInteger() ) }.join()
//	log.debug "IP address entered is $ipAddress and the converted hex code is $hex"
	return hex
}

private String convertPortToHex(port) {
	String hexport = port.toString().format( '%04x', port.toInteger() )
//	log.debug "Port entered is $port and the converted hex code is $hexport"
	return hexport
}

private Integer convertHexToInt(hex) {
	Integer.parseInt(hex,16)
}

private String convertHexToIP(hex) {
	//log.debug("Convert hex to ip: $hex")
	[convertHexToInt(hex[0..1]),convertHexToInt(hex[2..3]),convertHexToInt(hex[4..5]),convertHexToInt(hex[6..7])].join(".")
}

private getHostAddress() {
	def parts = device.deviceNetworkId.split(":")
	//log.debug device.deviceNetworkId
	def ip = convertHexToIP(parts[0])
	def port = convertHexToInt(parts[1])
	return ip + ":" + port
}

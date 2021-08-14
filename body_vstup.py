import gpxpy
import gpxpy.gpx

gpx_file = open('body.gpx', 'r')
gpx = gpxpy.parse(gpx_file)

with open('input_body.txt', 'a') as f:
	f.write('{')
	for i, bod in enumerate(gpx.waypoints):
		if i != len(gpx.waypoints) - 1:
			f.write('{' + str(bod.latitude) + ', ' + str(bod.longitude) + '}, ')
		
		else:
			f.write('{' + str(bod.latitude) + ', ' + str(bod.longitude) + '}}')



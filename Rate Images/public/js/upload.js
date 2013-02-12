$(function() {
	$('#file').fileupload({
		dataType: 'json',
		formData: function() {
			return this.context.find('form').serializeArray();
		},
		add: function(e, data) {
			$('#file').fileupload('process', {
				files: data.files,
				process: [
					{action: 'load', fileTypes: /^image\/(gif|jpe?g|png)$/},
					{action: 'resize', maxWidth: 2400, maxHeight: 2300},
					{action: 'save', type: 'image/jpeg'}
				],
			}).done(function() {
				var reader = new FileReader();
				reader.onload = function(e) {
					var template = _.template($('#image-panel-template').text()),
						src = e.target.result,
						html = template({src: src});

					var li = data.context = $('<li/>').html(html).appendTo('.forms-container');
					li.find('form').submit(function(e) {
						data.submit();
						li.find('button[type="submit"]').hide();
						e.preventDefault();
						return false;
					});
				}
				reader.readAsDataURL(data.files[0]);
			});
		},
		done: function(e, data) {
			data.context.find('.upload-percent').html('&#x2713;&nbsp;&nbsp; Uploaded 100%');
		},
		progress: function(e, data) {
			data.context.find('.upload-percent').text('Uploaded ' + Math.round(data.loaded*100/data.total) + '%');
		}
	});
});
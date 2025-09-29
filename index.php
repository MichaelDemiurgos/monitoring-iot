<?php
include "koneksi.php";

// ambil log terakhir
$result = mysqli_query($conn, "SELECT * FROM sensor_log ORDER BY created_at DESC LIMIT 15");
$data = [];
while ($row = mysqli_fetch_assoc($result)) {
    $data[] = $row;
}

// ambil status lampu
$lampu = mysqli_query($conn, "SELECT * FROM lampu");
$lamp_status = [];
while ($row = mysqli_fetch_assoc($lampu)) {
    $lamp_status[] = $row;
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Dashboard Monitoring Kebisingan</title>
  <link rel="stylesheet" href="style.css">
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <meta http-equiv="refresh" content="10">
</head>
<body>
<header>
  <h1>Dashboard Monitoring Kebisingan (KY-037)</h1>
</header>

<div class="container">

  <div class="card">
    <h2>Status Lampu Indikator</h2>
    <div class="lampu-status">
      <?php foreach ($lamp_status as $lamp): ?>
        <div class="lamp <?php echo $lamp['status']=='ON' ? 'on ' . strtolower(explode(' ', $lamp['lamp_name'])[1]) : ''; ?>">
          <?php echo $lamp['lamp_name']; ?>
        </div>
      <?php endforeach; ?>
    </div>
  </div>

  <div class="card">
    <h2>Grafik Kebisingan Suara</h2>
    <canvas id="myChart"></canvas>
  </div>

  <div class="card">
    <h2>Log Data Terbaru</h2>
    <table>
      <tr>
        <th>Waktu</th>
        <th>Level Suara</th>
        <th>Status</th>
        <th>Lampu</th>
      </tr>
      <?php foreach ($data as $d): ?>
      <tr>
        <td><?php echo $d['created_at']; ?></td>
        <td><?php echo $d['sound_level']; ?></td>
        <td><?php echo $d['sound_status']; ?></td>
        <td><?php echo $d['lamp_id']; ?></td>
      </tr>
      <?php endforeach; ?>
    </table>
  </div>

</div>

<script>
const labels = <?php echo json_encode(array_column($data, 'created_at')); ?>;
const values = <?php echo json_encode(array_column($data, 'sound_level')); ?>;

const ctx = document.getElementById('myChart').getContext('2d');
new Chart(ctx, {
  type: 'line',
  data: {
    labels: labels.reverse(),
    datasets: [{
      label: 'Level Suara',
      data: values.reverse(),
      borderColor: '#3498db',
      backgroundColor: 'rgba(52,152,219,0.3)',
      fill: true,
      tension: 0.3
    }]
  },
  options: {
    responsive: true,
    plugins: {
      legend: { display: true }
    },
    scales: {
      y: {
        beginAtZero: true,
        max: 1023
      }
    }
  }
});
</script>
</body>
</html>

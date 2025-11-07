<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>PlanXY - FREE FET-Powered Timetabling Software for Modern Windows</title>

  <!-- Tailwind CDN -->
  <script src="https://cdn.tailwindcss.com"></script>

  <!-- Alpine.js CDN -->
  <script defer src="https://cdn.jsdelivr.net/npm/alpinejs@3.x.x/dist/cdn.min.js"></script>

  <style>
    html { scroll-behavior: smooth; }
  </style>
</head>

<body class="font-sans text-gray-800">

  <!-- Navbar -->
  <header class="fixed w-full bg-white/90 backdrop-blur shadow-sm z-50 transition">
    <nav class="max-w-6xl mx-auto flex justify-between items-center p-4">
      <a href="#home" class="flex items-center space-x-2">
        <img src="app.svg" width="48" height="48" alt="PlanXY logo" />
        <span class="text-xl font-bold text-blue-700">Plan XY</span>
      </a>
      <ul class="hidden md:flex space-x-6 font-medium">
        <li><a href="#home" class="hover:text-blue-600 transition">Home</a></li>
        <li><a href="#features" class="hover:text-blue-600 transition">Features</a></li>
        <li><a href="#download" class="hover:text-blue-600 transition">Download</a></li>
        <li><a href="#copyright" class="hover:text-blue-600 transition">Copyright</a></li>
      </ul>

      <!-- Mobile Menu -->
      <div x-data="{ open: false }" class="md:hidden relative">
        <button @click="open = !open">
          <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none"
               viewBox="0 0 24 24" stroke="currentColor">
            <path x-show="!open" stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                  d="M4 6h16M4 12h16M4 18h16" />
            <path x-show="open" stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                  d="M6 18L18 6M6 6l12 12" />
          </svg>
        </button>

        <ul x-show="open" x-transition
            @click.outside="open = false"
            class="absolute right-0 mt-3 w-40 bg-white border border-gray-200 shadow-lg rounded-lg p-2 flex flex-col space-y-2">
          <li><a href="#home" class="block px-3 py-1 rounded hover:bg-gray-100">Home</a></li>
          <li><a href="#features" class="block px-3 py-1 rounded hover:bg-gray-100">Features</a></li>
          <li><a href="#download" class="block px-3 py-1 rounded hover:bg-gray-100">Download</a></li>
          <li><a href="#copyright" class="block px-3 py-1 rounded hover:bg-gray-100">Copyright</a></li>
        </ul>
      </div>
    </nav>
  </header>


  <!-- Hero Section -->
<section id="home" class="relative min-h-screen grid md:grid-cols-[2fr_1fr] items-center text-white overflow-hidden pt-16">
    <div class="absolute inset-0 bg-gradient-to-r from-blue-500 via-purple-500 to-blue-600"></div>

  <!-- LEFT: Image Carousel -->
<div 
  x-data="{ active: 0, images: ['1.jpg','2.jpg','3.jpg','4.jpg','5.jpg'] }"
  x-init="setInterval(() => active = (active + 1) % images.length, 5000)"
  class="relative w-full flex justify-center items-center">

  <!-- Container for images -->
  <div class="relative w-11/12 md:w-5/6 h-[60vh] md:h-[80vh] rounded-2xl overflow-hidden shadow-2xl">
    <template x-for="(img, i) in images" :key="i">
      <img 
        x-show="active === i"
        :src="img"
        class="absolute inset-0 w-full h-full object-cover transition-opacity duration-1000 ease-in-out"
        x-transition:enter="opacity-0"
        x-transition:enter-end="opacity-100"
        x-transition:leave="opacity-100"
        x-transition:leave-end="opacity-0"
        alt="Carousel Image">
    </template>

    <!-- Dots BELOW the image -->
    <div class="absolute bottom-4 left-1/2 -translate-x-1/2 flex space-x-2">
      <template x-for="(img, i) in images" :key="i">
        <button 
          @click="active = i"
          :class="active === i ? 'bg-white' : 'bg-white/50'"
          class="w-3 h-3 rounded-full transition"></button>
      </template>
    </div>
  </div>

</div>


    <!-- Right: Text -->
    <div class="relative z-10 text-center md:text-left space-y-6 px-8">
      <h1 class="text-4xl md:text-6xl font-bold drop-shadow-md">Plan XY</h1>
      <p class="text-lg md:text-2xl">FET-powered modern UI Timetabling Software for Windows 10/11.</p>
      <p class="text-base">
        Powered by the powerful 
        <a href="https://lalescu.ro/liviu/fet/" target="_blank" class="underline hover:text-blue-200">FET</a>, 
        Plan XY is your perfect timetable generator. FREE. 
      </p>
      <div class="space-x-4 pt-4">
        <a href="#features" class="bg-white text-blue-600 px-6 py-3 rounded-full font-semibold hover:bg-gray-100 transition">Features</a>
        <a href="#download" class="bg-white text-blue-600 px-6 py-3 rounded-full font-semibold hover:bg-gray-100 transition">Download</a>
      </div>
    </div>
  </section>

  <!-- Features Section -->
  <section id="features" class="py-20 bg-gray-50 text-center">
    <h2 class="text-3xl font-bold mb-12">Features</h2>
    <div class="max-w-6xl mx-auto grid grid-cols-1 md:grid-cols-4 gap-8 px-4">
      <div class="p-6 bg-white rounded-xl shadow-md hover:shadow-xl hover:-translate-y-1 transition transform">
        <h3 class="text-xl font-semibold mb-2">Interface</h3>
        <div class="h-0.5 bg-blue-600 w-12 mx-auto mb-4"></div>
        <ul class="list-disc list-inside text-left text-gray-600 space-y-2">
          <li>Modern WinUI 3 interface for entering unlimited data.</li>
          <li>Real-time collaboration with other users.</li>
          <li>9 languages.</li>
          <li>Automatic updates.</li>
        </ul>
      </div>
      <div class="p-6 bg-white rounded-xl shadow-md hover:shadow-xl hover:-translate-y-1 transition transform">
        <h3 class="text-xl font-semibold mb-2">Data Input</h3>
        <div class="h-0.5 bg-blue-600 w-12 mx-auto mb-4"></div>
        <ul class="list-disc list-inside text-left text-gray-600 space-y-2">
          <li>Single-click interface for quick entry.</li>
          <li>Input statistics.</li>
          <li>CSV import support.</li>
        </ul>
      </div>
      <div class="p-6 bg-white rounded-xl shadow-md hover:shadow-xl hover:-translate-y-1 transition transform">
        <h3 class="text-xl font-semibold mb-2">Process Power</h3>
        <div class="h-0.5 bg-blue-600 w-12 mx-auto mb-4"></div>
        <ul class="list-disc list-inside text-left text-gray-600 space-y-2">
          <li>FET-powered super speed timetable generation.</li>
          <li>Multi-threaded timetable generation.</li>
          <li>Multiple timetables simultaneously.</li>
          <li>Notifications when completed.</li>
        </ul>
      </div>
      <div class="p-6 bg-white rounded-xl shadow-md hover:shadow-xl hover:-translate-y-1 transition transform">
        <h3 class="text-xl font-semibold mb-2">Data Output</h3>
        <div class="h-0.5 bg-blue-600 w-12 mx-auto mb-4"></div>
        <ul class="list-disc list-inside text-left text-gray-600 space-y-2">
          <li>Export to HTML per teacher/class/room or combined.</li>
          <li>Output statistics overview.</li>
          <li>Export compatible with FET.</li>
        </ul>
      </div>
    </div>
  </section>

  <!-- Download Section -->
  <section id="download" class="py-20 bg-white text-center">
    <h2 class="text-3xl font-bold mb-6">Download</h2>
    <p class="max-w-2xl mx-auto text-gray-600 mb-8">
      Download the auto-updating single executable for Windows 10/11 x64 below.
    </p>
    <a href="https://www.turbo-play.com/update2/tu.php?p=f3cf159b-de75-4427-8fe0-81a7ae61d3fa&f=29410c69-7a2b-4a2b-9893-9de712a5cd77"
       class="inline-block bg-blue-600 text-white px-8 py-3 rounded-full font-semibold hover:bg-blue-700 transition">
      Download for Windows 10/11
    </a>
	<br><br>
    <p class="max-w-2xl mx-auto text-gray-600 mb-8">
      Download from the App Store.
    </p>
	<a href="https://apps.microsoft.com/detail/9nfv4333x7jr?hl=en&gl=GR&ocid=pdpshare" class="inline-block bg-blue-600 text-white px-8 py-3 rounded-full font-semibold hover:bg-blue-700 transition">Download from Microsoft Store</a>
  </section>

  <!-- Copyright Section -->
  <section id="copyright" class="py-16 bg-gray-50 text-center text-sm text-gray-600 space-y-2">
    <p>
      FET is © by Liviu Lalescu and Volker Dirr. Visit the FET homepage 
      <a href="https://lalescu.ro/liviu/fet/" target="_blank" class="text-blue-600 font-semibold underline hover:text-blue-800">here</a>.
    </p>
    <p>
      PlanXY is open source — see the code 
      <a href="https://github.com/WindowsNT/PlanXY" target="_blank" class="text-blue-600 font-semibold underline hover:text-blue-800">here</a>.
    </p>
  </section>

  <!-- Footer -->
  <footer class="py-6 bg-white text-center text-gray-500 text-sm border-t">
    &copy; 2025 Turbo-Play.COM. All rights reserved.
  </footer>

  <!-- Small Navbar Shadow on Scroll -->
  <script>
    window.addEventListener('scroll', () => {
      document.querySelector('header').classList.toggle('shadow-lg', window.scrollY > 20);
    });
  </script>

</body>
</html>

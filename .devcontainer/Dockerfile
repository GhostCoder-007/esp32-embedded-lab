FROM espressif/idf:release-v5.2

ARG DEBIAN_FRONTEND=nointeractive
ARG CONTAINER_USER=esp-bn
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN apt-get update \
  && apt install -y \
  cmake \
  git \
  hwdata \
  tree \
  udev \
  usbutils \
  libglib2.0-0 \
  libnuma1 \
  libpixman-1-0 \
  linux-tools-virtual \
  telnet \
  protobuf-c-compiler \
  libprotobuf-c-dev \
  doxygen \
  cairosvg \
  astyle \
  ripgrep \
  dfu-util \
  libssl-dev \
  zsh \
  ripgrep \
  && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/local/bin/usbip usbip `ls /usr/lib/linux-tools/*/usbip | tail -n1` 20

# Install all the required tools
RUN /opt/esp/idf/install.sh --enable-gdbgui
RUN /opt/esp/idf/tools/idf_tools.py install esp-clang


ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8
ENV TERM xterm-256color

RUN groupadd --gid $USER_GID $CONTAINER_USER \
    && adduser --uid $USER_UID --gid $USER_GID --disabled-password --gecos "" ${CONTAINER_USER} \
    && usermod -a -G root,dialout,plugdev $CONTAINER_USER

RUN chmod -R 775 /opt/esp/python_env/

USER ${CONTAINER_USER}
ENV USER=${CONTAINER_USER}
WORKDIR /home/${CONTAINER_USER}

RUN git config --global --add safe.directory /opt/esp/idf
RUN git config --global --add safe.directory /opt/esp/idf/components/openthread/openthread
RUN /opt/esp/python_env/idf5.2_py3.10_env/bin/python -m pip install esp_debug_backend
RUN /opt/esp/python_env/idf5.2_py3.10_env/bin/python -m pip install esp_docs

RUN echo "source /opt/esp/idf/export.sh > /dev/null 2>&1" >> ~/.bashrc
ENTRYPOINT [ "/opt/esp/entrypoint.sh" ]

CMD ["/bin/bash", "-c"]
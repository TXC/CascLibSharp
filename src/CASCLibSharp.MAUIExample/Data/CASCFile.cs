using System.ComponentModel;

namespace CASCLibSharp.MAUIExample
{
    public class CASCFile(string fileName, long fileSize, bool isLocal) : INotifyPropertyChanged
    {
#pragma warning disable IDE1006 // Naming Styles
        private string _FileName { get; set; } = fileName;
        private long _Size { get; set; } = fileSize;
        private bool _IsLocal { get; set; } = isLocal;
#pragma warning restore IDE1006 // Naming Styles

        public string FileName
        {
            get
            {
                return _FileName;
            }
            set
            {
                _FileName = value;
                OnPropertyChanged("FileName");
            }
        }

        public long Size
        {
            get
            {
                return _Size;
            }
            set
            {
                _Size = value;
                OnPropertyChanged("Size");
            }
        }

        public bool IsLocal
        {
            get
            {
                return _IsLocal;
            }
            set
            {
                _IsLocal = value;
                OnPropertyChanged("IsLocal");
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }
}
